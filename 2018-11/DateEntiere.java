/**
 * TP-DateEntiere (2018-11-14)
 * In short:
 * - The user enters a date
 * - The date may be of the form DAY-MONTH-YEAR or DAY-MONTH-YEAR
 * - 'Month' can be written as a number (e.g. '03') or a string (e.g. 'Mars')
 * - The user may confuse the day and month, attempt to fix
 * - "Parse" the date, validate it, then print it expressed in words
 *
 * My first time using RegEx in Java; other than that, there's
 *   nothing impressive about it. #UGLY #UNTIDY
 */
import java.util.Scanner;
import java.util.regex.*;

public class DateEntiere {
  
    public static void main(String[] args) {
    // Scanner sc = new Scanner(System.in);
    // String s = sc.nextLine();
    // sc.close();
    String s = "Today is 14-11-2018.";
    DateEntiere d = new DateEntiere(s);
    if (d.DateValide()) {
     System.out.println("La date en lettres: " + DateEntiere.enLettres(s));
    } else {
     System.out.println("La date est invalide: " + s);
    }
  }

  final static String[] mois = {"Janvier","Février","Mars","Avril","Mai","Juin","Juillet","Aout","September","Octobre","Novembre","Décembre"};
  final static String rNormal = ".*(\\d\\d)-(\\d\\d)-(\\d\\d\\d\\d).*";
  final static String rReversed = ".*(\\d\\d\\d\\d)-(\\d\\d)-(\\d\\d).*";
  
  String s;
  int[] parts;
  boolean valide = true;
  
  public DateEntiere (String s) {
    this.s = s;
    convertMonthToNumber();
    if (!this.s.matches(rNormal) && !this.s.matches(rReversed)) {
      this.valide = false;
    } else {
      if (this.s.matches(rNormal)) {
        toIntParts(rNormal);
      } else { // this.s.matches(rReversed)
        toIntParts(rReversed);
        // Correct it: année <-> jour
        int temp = parts[0];
        parts[0] = parts[2];
        parts[2] = temp;
      }
      fixMonthAndDay();
      int mois = parts[1];
      if (parts[0] > getMaxDays(mois) || !(mois >= 1 && mois <= 12))
        valide = false;
    }
  }

  private void convertMonthToNumber() {
    // "Today is 14 Novembre 2018 yo!" -> "Today is 14-11-2018 yo!"
    // "12 Avril 2013" -> "12-04-2013"
    for (int i = 0; i < 12; i++) {
      String num = String.format("-%02d-", i+1);
      s = s.replaceAll( " "+mois[i]+" ", num);
    }
  }
  
  private void toIntParts(String regex) {
    int[] result = new int[3];
    
    // Prepare the regex (Pattern), set the string, then find the parts/groups
    Pattern p = Pattern.compile(regex);
    Matcher m = p.matcher(s);
    m.find();
    // Now convert parts (strings) to numbers and save them in the result array
    result[0] = Integer.parseInt( m.group(1) );
    result[1] = Integer.parseInt( m.group(2) );
    result[2] = Integer.parseInt( m.group(3) );
    
    this.parts = result;
  }
  
  void fixMonthAndDay() {
    // if month is incorrect then maybe the user confused it with the day:
    // Day <-> Month
    if (parts[1] > 12) {
      int tmp = parts[0];
      parts[0] = parts[1];
      parts[1] = tmp;
    }
  }

  private static int getMaxDays(int m) {
    String fih31 = " 1 3 5 7 8 12 ";
    String fih30 = " 4 6 9 11 ";
    if (fih31.contains(" "+m+" ")) {
      return 31;
    } else if (fih30.contains(" "+m+" ")) {
      return 30;
    } else {
      return 28; // or 29
    }
  }
  
  public boolean DateValide() {
    return valide;
  }
  
  public static int getJour(String s) {
    DateEntiere d = new DateEntiere(s);
    if (d.valide)
      return d.parts[0];
    else 
      return -1;
  }
  
  public static int getMois(String s) {
    DateEntiere d = new DateEntiere(s);
    if (d.valide)
      return d.parts[1];
    else 
      return -1;
  }  
  
  public static int getAnnée(String s) {
    DateEntiere d = new DateEntiere(s);
    if (d.valide)
      return d.parts[2];
    else 
      return -1;
  }
  
  public static String enLettres(String s) {
    String result = getJour(s) + " " +  mois[ getMois(s)-1 ] + " " + getAnnée(s);
    return result;
  }
  
}
