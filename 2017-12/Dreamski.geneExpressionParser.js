var Dreamski = Dreamski || {};

Dreamski.geneExpressionParser = function (input) {
  /**
   * A shitty function to convert gene expressions to JS math expressions
   *
   * @example
   * parse('/+i*2c3') -> '((i)+((2)*(c)))/(3)' // '(i+2*c)/3' \(>_<)/
   *
   * Check https://en.wikipedia.org/wiki/gene_expression_programming
   */

  let stack = [];
  function parse(str) {
    if (str) stack = str.split('').reverse();
    const c = stack.pop();

    if (!c) return '';
    if (!['+', '-', '*', '/', '%'].includes(c)) return c;
    return '(' + parse() + ')' + c + '(' + parse() + ')';
  }

  return parse(input);
};
