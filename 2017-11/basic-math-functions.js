function mul(x,y){
  var z = 0;//result
  while(y > 0){
    z = z + x;
    y = y - 1;
    };
  return z;
}

function div(x,y){
  var z = 0;
  while(x >= y){
    x = x - y;
    z = z + 1;
    };
  return z;
}

function pow(x,y){
  var z = 1;
  while(y>0){z = z*x; y = y - 1;}
  return z;
}

function abs(x){
  if( x > 0 ){
    return x;
  } else {
    return -x;
  }
}

function mod(x,y){
  while(x >=y ) x = x - y;
  return x;
}

function floor(x){
  return x - mod(x,1);
}

function exo3(n){
  while(n>1) console.log((n = n%2==0 ? n/2 : n*2+1));
  return n;
}
