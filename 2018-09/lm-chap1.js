/**
 * LM-Chap1 -- 2018-09-05
 * parts: Lexer, Parser, Drawer, Main
 * Check `lm-chap1.html` fo~ mo~ info~
 */

// ============ LEXER =========
class Token {
    constructor(type, value) {
      this.type = type
      this.value = value || null
    }
}

class Lexer {
  constructor(input) {
    this.text = input
    this.position = 0
    this.currentChar = this.text[0] || null
  }
  
  lex() {
    const result = [];
    let token;
    while (token = this.getNextToken()) {
      result.push(token);
      if (token.type === 'NULL') break;
    }
    return result;
  }
    
  next() {
    this.position++
    this.currentChar = this.text[this.position] || null
  }
  
  match(c) {
    if (c === this.currentChar) {
      this.next();
    } else {
      throw Error(`Expected ${c} at ${this.position}, instead got ${this.currentChar}`);
    }
  }

  getNextToken() {
    if (this.currentChar === null) {
      return new Token('NULL')
    }
    else if (/[A-Z]/.test(this.currentChar)) {
      return this.variable()
    }
    else if (this.currentChar === '!') {
      this.next()
      return new Token('NEG', '!')
    }
    else if (this.currentChar === '(') {
      this.next()
      return new Token('LPR', '(')
    }
    else if (this.currentChar === ')') {
      this.next()
      return new Token('RPR', ')')
    }
    else if (this.currentChar === '*') {
      this.next()
      return new Token('ET', '*')
    }
    else if (this.currentChar === '+') {
      this.next()
      return new Token('OU', '+')
    }
    else if (this.currentChar === '-') {
      this.next()
      this.match('>')
      return new Token('IMPLI', '->')
    }
    else if (this.currentChar === '<') {
      this.next()
      this.match('-')
      this.match('>')
      return new Token('EQUI', '<->')
    }
    else {
      throw new Error(`Unknown token '${this.currentChar}' at position ${this.position}`)
    }
  }

  variable() {
    let val = '';
    while (/[A-Z]/.test(this.currentChar)) {
      val += this.currentChar;
      this.next();
    }
    while (/[0-9]/.test(this.currentChar)) {
      val += this.currentChar;
      this.next();
    }
    while (/'/.test(this.currentChar)) {
      val += this.currentChar;
      this.next();
    }
    return new Token('VAR', val);
  }

}

// =================== PARSER =================
// <AST>
class Connecteur {
    constructor() {}
}

class Binary extends Connecteur {
  constructor(token, left, right) {
    super()
    this.left = left
    this.val = token.type
    this.right = right
  }
}

class Unary extends Connecteur {
  constructor(token, branch) {
    super()
    this.val = token.type
    this.branch = branch
  }
}

class Var extends Connecteur {
  constructor(token) {
    super()
    this.val = token.value
  }
}

// </AST>

class Parser {
  constructor(tokens) {
    tokens.map(console.info)
    this.tokens = tokens
    this.currentToken = this.tokens.shift()
  }

  parse() {
    const ast = this.expr()
    if (this.currentToken.type !== 'NULL')
      throw Error(`Unexpected token ${this.currentToken.type}`)
    return ast
  }

  next() {
    this.currentToken = this.tokens.shift()
  }
  
  match(t) {
    if (this.currentToken.type === t) {
        this.next()
    } else {
        throw Error(`Expected "${t}", instead got "${this.currentToken.type}"`)
    }
  }

  variable() {
    const node = new Var(this.currentToken)
    this.match('VAR')
    return node
  }

  expr() {
    let result = this.term()

    while (['IMPLI', 'EQUI'].includes(this.currentToken.type)) {
      let token = this.currentToken
      this.next()
      result = new Binary(token, result, this.term())
    }

    return result
  }

  term() {
    // term : factor (( ET | OU ) factor)*

    let result = this.factor()

    while (['ET', 'OU'].includes(this.currentToken.type)) {
      const token = this.currentToken
      this.next();
      result = new Binary(token, result, this.factor())
    }

    return result
  }

  factor() {
    // factor : NEG factor
    //        | LPR expr RPR
    //        | variable

    const token = this.currentToken

    if (token.type === 'NEG') {
      this.match('NEG')
      return new Unary(token, this.expr())
    } else if (token.type === 'LPR') {
      this.match('LPR')
      const result = this.expr()
      this.match('RPR')
      return result
    } else {
      console.info(token)
      return this.variable()
    }
  }
}

// ================== DRAWER ======================
function Drawer(ast) {
	class Node {
		constructor(x, y, text) {
			this.x = x;
			this.y = y;
			this.g =
			`<g class="node">
				<ellipse cx="${x}" cy="${y}"/>
				<text x="${x}" y="${y + 5}">${text}</text>
			</g>`;
		}
	}
	
	class Edge {
		constructor(a, b) {
			this.a = a;
			this.b = b;
			this.g =
			`<g class="edge">
				<path d="M ${a.x} ${a.y + 15} l ${a.x - b.x} 15"/>
			</g>`;
		}
	}
	
	const result = []

	function process(ast, parent, _x = 0, _y = 40) {

		const x = parent.x + _x;
		const y = parent.y + _y;
		const node = new Node(x, y, ast.val)
		const edge = new Edge(parent, node)
		result.push(edge, node)

		if (ast instanceof Binary) {
			/******************
			* 
			*     (parent)     
			*    /        \    
			*  (L)        (R)  
			*                 
			******************/
			process(ast.left, node, -70);
			process(ast.right, node, +70);
		}
		else if (ast instanceof Unary) {
			/*****************
			*                 
			*    (parent)     
			*       |         
			*	   (N)        
			*                 
			******************/			
			process(ast.branch, node);
		}
		else {
			// if it's Var, then it was already handled
		}
	}
	
	root = new Node(250 , -20, 'EXPRESSION');
	result.push(root);
	process(ast, root);
	
	result.shift(); result.shift(); // remove the root's node and edge
	return [
			...result.filter( x => x instanceof Edge),
			...result.filter( x => x instanceof Node)
			].map( x => x.g ).join('\n');
}

// ================ MAIN ================
/*
By priority
'\u00AC' ¬ !
'\u2227' ∧ *
'\u2228' ∨ +
'\u2192' → ->
'\u2194' ↔ <->
*/

function format(str) {
  const map = 
  {
    '!': '¬',
    '*': '∧',
    '+': '∨',
    '->': '→',
    '<->': '↔'
  };
  
  return (str || '')
    .toUpperCase()
    .replace(/\s+/g, '')
    .replace(/(\!|\*|\+|<->|->)/g, x => map[x]);
}

const $input = document.getElementById('input');
const $formatted = document.getElementById('formatted');
const $graph = document.getElementById('graph');

function update() {
  try {
    const input = $input.value;
    $formatted.innerHTML = format(input);
    const tokens = new Lexer(input).lex();
    const tree = new Parser(tokens).parse();
    $graph.innerHTML = Drawer(tree);
    $input.title = '';
    $input.classList.remove('err');
  } catch (e) {
    console.error(e);
    $input.title = e;
    $input.classList.add('err');
  }  
}

$input.oninput = update;

update();
