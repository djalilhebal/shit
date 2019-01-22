function bin_to_oct(bin){

  const div = (x,y) => Math.floor(x/y);
  var oct = 0, ooo, o = 1; // to catch zeros (we'll remove this "1" later)
  
  while(bin != 0){
    ooo = bin%1000;
    switch(ooo){
    case   0: o = o*10 + 0; break;
    case   1: o = o*10 + 1; break;
    case  10: o = o*10 + 2; break;
    case  11: o = o*10 + 3; break;
    case 100: o = o*10 + 4; break;
    case 101: o = o*10 + 5; break;
    case 110: o = o*10 + 6; break;
    case 111: o = o*10 + 7; break;
    }

    bin = div(bin, 1000);
  }
  while(o != 0){oct = oct*10 + o%10; o = div(o, 10);}
  oct = div(oct, 10); //removing that "1"
  return oct;
};