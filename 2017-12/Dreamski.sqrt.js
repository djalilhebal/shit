var Dreamski = Dreamski || {};

Dreamski.sqrt = function sqrt(val) {
  // An unefficient sqrt function :p

  if (val < 0) return -1; // Like, an error ._.

  let x = 0;
  let step = val < 1 ? 0.1 : 10;

  function guessNewX(oldX) {
    if (oldX * oldX > val) {
      return oldX - step;
    }
    return oldX + step;
  }

  while (x * x != val && step > 0.00000001) {
    // Going to go forth and back in our guess? 'step' is too large, reduce it!
    if (guessNewX(guessNewX(x)) == x) step /= 10;
    // Okay, guess our 'x' now
    x = guessNewX(x);
  }

  return x;
};
