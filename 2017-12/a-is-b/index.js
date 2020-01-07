/// This thing is buggy/limited/messy. I'M A GROWN MAN, I KNOW MY SHIT!

var db  = {};  // "database/knowledgebase"

var dic = {    // "dictionary"
  entries: [], // [ {'+good', '-bad'}, {'-good', '+bad'} ]
  
  indexOf: function(x) {
    let val = -1;
    dic.entries.forEach(function(d, i){
      if (d.has(x)) return (val = i);
    });
    return val;
    },

  addPair: function(x, y) {
    let xi = dic.indexOf(x), yi = dic.indexOf(y);
    if (xi < 0 && yi <0) return dic.entries.push( new Set([x, y]) );
    if (xi >= 0) return dic.entries[xi].add(y);
    if (yi >= 0) return dic.entries[yi].add(x);
    },
    
  getSyn: function (x) { // get an array of synonyms
    let i = dic.indexOf(x);
    if(i < 0) return [x];
    return [ ...dic.entries[i] ];
  }

}

function parse(input){
  const rQ     = /\?/;
  const rBe    = /^(\w+) be (.+)/i;
  const rBeQ   = /^be (\w+) (.+)/i;
  const rMean  = /^(\w+) means (.+)/i;
  const rMeanQ = /^does (\w+) mean (.+)/i;
  
  let simplified   = input
      .replace(/\b((is|are|ai)n't)\b/g, 'be not')
      .replace(/\b(is|am|are)\b/g, 'be')
      .replace(/[^\s\w]/g, '');
  
  // resimplify the sentence by removing 'not' and doing whitespaces 
  let s            = simplified.replace(/\bnot\b/ig, '').replace(/\s+/g, ' ').trim();
  let isNegated    = (simplified.match(/\bnot\b/ig) || []).length % 2 == 1;
  let isQuestion   = rQ.test(input) || rBeQ.test(s) || rMeanQ.test(s);
  let isDefinition = rMean.test(s) || rMeanQ.test(s);
  // 'malice be/means evil' => A = malice, B = evil
  let [, A, B] = s.match(rBe) || s.match(rBeQ) || s.match(rMean) || s.match(rMeanQ) || [];
  
  return { A, B, isQuestion, isNegated, isDefinition };
}

function handleMean(parsed){
  const {A, B, isNegated, isQuestion} = parsed;
  
  let Ap, Bp, An, Bn;
  if (isNegated){
    Ap = '+' + A; Bp = '-' + B; // +good = -bad
    An = '-' + A; Bn = '+' + B; // -good = +bad
  } else {
    Ap = '+' + A; Bp = '+' + B; // +happy = +content
    An = '-' + A; Bn = '-' + B; // -happy = -content
  }
  
  if (isQuestion){
    if (dic.indexOf(Ap) < 0 || dic.indexOf(Bp) < 0) return 'Dunno';
    if (dic.indexOf(Ap) == dic.indexOf(Bp)) return 'Yeah';
    return 'Nah';
  } else {
    dic.addPair(Ap, Bp);
    dic.addPair(An, Bn);
    return 'Got it!';  
  }
}

function handleBe(parsed){
  const {A, B, isNegated, isQuestion} = parsed;
  
  let adj = (isNegated ? '-' : '+' ) + B;
  let neg = (isNegated ? '+' : '-' ) + B;
  
  if (isQuestion){ // handle questions
    if ( !db[A] ) return 'I know nothing about ' + A;
    if ( dic.getSyn(adj).some( x => db[A].has(x) ) ) return 'Yup';
    if ( dic.getSyn(neg).some( x => db[A].has(x) ) ) return 'Nope';
    return 'IDK';
  } else { // handle statements
    if ( !db[A] ) db[A] = new Set();
  dic.getSyn(neg).forEach( x => db[A].delete(x) );
    db[A].add(adj);
    return 'Okay';
  }
}

function update(){
  let input  = document.getElementById('input');
  let output = document.getElementById('output');

  let parsed = parse(input.value.toLowerCase());

  let reply  = 'What?';
  if (parsed.A && parsed.B)
    reply = parsed.isDefinition ? handleMean(parsed) : handleBe(parsed);

  let tmp = document.createElement('div');
  tmp.innerHTML = '<p class=user>' + input.value + '</p><p class=reply>' + reply + '</p>';

  input.value = ''; // clear the input
  output.appendChild(tmp); // show the input/message and reply bubbles
  output.scrollTop = output.scrollHeight;
}
