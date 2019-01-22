// leo-xo.js -- 2018-07-31
// I can't even claim that this messy code is inspired by the MVC Pattern (>_<)
// Gotta edit it later~~~

let game = null;

function Game() {

	const states = [];
	const board = Array(9).fill('-');
	let validMoves = [0, 1, 2, 3, 4, 5, 6, 7, 8];
	let turn = 'X';
		
	function isValidMove(n) {
		return validMoves.indexOf(n) >= 0;
	}
	
	function getRandomValidMove() {
		const i = Math.floor( validMoves.length * Math.random() );
		return validMoves[i];
	}

	function setCell(n) {
		if (!isValidMove(n)) throw new Error('invalid move: ' + n);
		
		const state = {
			board: board.slice(),
			turn: turn,
			m: n,
		};
		states.push(state);

		board[n] = turn;
		validMoves = validMoves.filter( m => m !== n);
		turn = turn == 'X' ? 'O' : 'X';
		
		view.drawCell(n);
	}
	
	function isWinner(c) {
		const WINS = [
		  [0, 1, 2],
		  [3, 4, 5],
		  [6, 7, 8],
		  [0, 3, 6],
		  [1, 4, 7],
		  [2, 5, 8],
		  [0, 4, 8],
		  [2, 4, 6]
		];
		return WINS.some( WIN => {
			return c == board[WIN[0]] && c == board[WIN[1]] && c == board[WIN[2]];
		});
	}
	
	function isFullBoard() {
		return validMoves.length === 0;
	}
	
	function isGameOver() {
		return isWinner('X') || isWinner('O') || isFullBoard();
	}

	function getWinner() {
		if (isWinner('X')) {
			return 'X'
		}
		else if (isWinner('O')){
			return 'O'
		}
		else {
			return '-'
		}
	}
	
	function getTurn() {
		return turn;
	}

	function getBoard() {
		return board.slice();
	}
	
	function getRecord() {
		return { states, winner: getWinner() }
	}
	
	return {
		getTurn,
		getWinner,
		getRandomValidMove,
		getBoard,
		getRecord,
		isValidMove,
		isGameOver,
		setCell,
	};
}

const view = (function View() {
	const $board = document.getElementById('board');
	const $replay = document.getElementById('replay');

	function drawCell(n) {
		const $cell = document.getElementById('cell-'+n);
		const c = game.getBoard()[n];
		$cell.removeAttribute('class'); // to remove class="valid"
		$cell.removeAttribute('onclick'); // to the "event handler"
		$cell.innerHTML = c;
	}
	
	function newTable() {
		let n = 0; // cell number
		let html = '<table id="table">';
		for (let i = 0; i < 3; i++) {
			html += '<tr>';
			for (let j = 0; j < 3; j++) {
				html += `<td id="cell-${n}" class="valid" onclick="manager.moveX(${n})">-</td>`;
				n++;
			}
			html += '</tr>';		
		}
		html += '</table>';
		return html;
	}
	
	function newBoard() {
		$board.innerHTML = newTable();		
	}

	function hideReplay() {
		$replay.hidden = true;		
	}
	
	function showReplay() {
		$replay.hidden = false;
	}
	
	function alertWinner() {
		const $table = document.getElementById('table');
		$table.className = 'game-over';
		const winner = game.getWinner();
		const msg = winner == '-' ? `CAT'S GAME` : `Winner: ${winner}`;
		alert(msg);	
	}

	return {
		newBoard,
		hideReplay,
		showReplay,
		drawCell,
		alertWinner,
	}
	
})();

const manager = (function Manager() {
	
	function reset() {
		game = new Game();
		view.newBoard();
		view.hideReplay();
	}

	function gameOver() {
		leo.learn();
		view.alertWinner();
		view.showReplay();
	}
	
	function moveX(num) {
		if (game.getTurn() == 'X' && game.isValidMove(num)) {
			game.setCell(num);
			if (game.isGameOver()) return gameOver();
		}
		moveO();
	}
	
	function moveO() {
		if (game.getTurn() === 'O') leo.go();
		if (game.isGameOver()) return gameOver();
	}
	
	return {
		reset,
		moveX,
	}
})();

// defined in a separate file. it's more or less an edited and simplified version of
// github.com/serendipious/nodejs-decision-tree-id3
function ID3() {} 

const leo = (function Leo() {
	// Variables that start with `_` are "private"
	const _records = [];
	const _training = [];
	let _dt = null;
	
	function go() {
		const obj = arrToObj(game.getBoard());
		let m = _dt ? _dt.traverse(obj) : -1;
		if (!game.isValidMove(m)) {
			console.log('RANDOM MOVE')
			m = game.getRandomValidMove();
		}
		game.setCell(m);
	}

	function learn() { // from the current game
		if (!game) throw Error('no game is found');
		if (!game.isGameOver()) throw Error('The game is not over yet');

		const r = game.getRecord();
		_records.push(r);

		const preprocessed = preprocessRecord(r);
		preprocessed.forEach( (record) => {
			onlyO(record).forEach( (state) => {
				// state = {board, turn, m}
				const unified = arrToObj(state.board);
				unified.m = state.m;
				unified.turn = state.turn;
				_training.push(unified);
			});
		});
		
		_dt = new ID3(_training, 'm', '012345678'.split(''));
	}

	function onlyO (r) { // keep only the winner's states
		return r.states.filter( state => state.turn === 'O')
	}
	
	function arrToObj(arr) {
		const obj = {};
		for (let i = 0; i < arr.length; i++) obj[i] = arr[i];
		return obj;
	}
	
	function rev(c) {
		if (c === 'X') return 'O'
		if (c === 'O') return 'X'
		return '-'
	}
	
	function reverseRecord(r) {
		const newR = {};
		newR.winner = rev(r.winner);
		newR.states = r.states.map( (state) => {
			return {
				board: state.board.map(rev),
				turn: rev(state.turn),
				m: state.m,
			}
		});
		return newR;
	}
		
	function preprocessRecord(r) {
		if (r.winner === 'O') {
			return [r];
		} else if (r.winner === 'X') {
			return [reverseRecord(r)]
		} else {
			return [r, reverseRecord(r)]
		}
	}
	
	function getRecords() {
		return _records;
	}
	
	return {
		go,
		learn,
		getRecords,
	}
	
})();
