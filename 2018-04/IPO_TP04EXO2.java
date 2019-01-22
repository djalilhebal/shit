// I can't believe my shit was being copied by students I didn't even know
import java.util.Scanner;

class Client {
  private String CIN, nom, prenom, tel;
  
  // Constructeur avec tous les attributs (CIN, nom, prenom, tel)
  public Client(String a, String b, String c, String d) {
    CIN = a; nom = b; prenom = c; tel = d;
  }
  
  // Constructeur avec CIN, nom, prenom
  public Client(String a, String b, String c) {
    this(a, b, c, "");
  }
  
  // Les methodes d'accès aux differents attributs de la classes
  public void setCIN(String x) { this.CIN = x; }
  public String getCIN() { return this.CIN; }

  public void setNom(String x) { this.nom = x; }
  public String getNom() { return this.nom; }

  public void setPrenom(String x) { this.prenom = x; }
  public String getPrenom() { return this.prenom; }
  
  public void setTel(String x) { this.tel = x; }
  public String getTel() { return this.tel; }

  public void afficher() {
    System.out.println("CIN: " + CIN);
    System.out.println("NOM: " + nom);
    System.out.println("Prenom: " + prenom);
    System.out.println("Tel: " + tel);
  }
}

class Compte {
  static int co = 0; 
  
  private int numero, solde;
  private Client client;
  
  public Compte(String CIN, String nom, String prenom, String tel) {
    client = new Client(CIN, nom, prenom, tel);
    numero = ++co;
    solde  = 0;
  }

  public Compte(String CIN, String nom, String prenom) {
    this(CIN, nom, prenom, "");
  }
  
  // le numero et le solde sont en lecture seule
  public int getNumero() { return this.numero; }
  public int getSolde() { return this.solde; }
  
  // une methode permettant de crediter le compte, prenant une somme en parametre
  public void crediter(int somme) {
    this.solde += somme;
    System.out.println("Operation bien effectuee");
  }
  
  // une methode permettant de debiter le compte, prenant une somme en parametre
  public void debiter(int somme) {
    if (somme > 0 && solde >= somme) {
      this.solde -= somme;
      System.out.println("Operation bien effectuee");
    } else {
      System.out.println("Operation pas effectuee");
    }
  }

  // une methode permettant de crediter le compte
  // prenant une somme et un compte en parametres,
  // creditant le compte et debitant le compte passe en parametres
  public void crediter(int somme, Compte x) {
    if (somme > 0 && x.getSolde() >= somme) {
      x.debiter(somme);
      this.solde += somme;
    }  else {
      System.out.println("Operation pas effectuee");
    }
  }

  // une methode permettant de debiter le compte,
  // prenant une somme et un compte en parametres,
  // debitant le compte et creditant le compte passe en parametres
  public void debiter(int somme, Compte x) {
    if (solde >= somme) {
      x.crediter(somme);
      this.solde -= somme;
      System.out.println("Operation bien effectuee");
    } else {
      System.out.println("Operation pas effectuee");
    }
  }
  
  public void afficherResume() {
    System.out.println("Details du compte:");    
    System.out.println("******************");
    System.out.println("Numero de Compte: " + numero);
    System.out.println("Solde de compte: " + solde);
    System.out.println("Proprietaire du compte :");
    client.afficher();
    System.out.println("********************");    
  }
  
  public void afficherNombreComptes() {
    System.out.println("Le nombre de comptes crees: " + co);
  }
}

public class IPO_TP04EXO2 {
  public static void main(String args[])  {
    Scanner sc = new Scanner(System.in);
    String a, b, c, d;
    int m; // montant/money
    
    System.out.println("Compte 1:");
    System.out.print("Donner Le CIN: "); a = sc.nextLine();
    System.out.print("Donner Le Nom: "); b = sc.nextLine();
    System.out.print("Donner Le Prenom: "); c = sc.nextLine();
    System.out.print("Donner Le numero de telephone: "); d = sc.nextLine();
    Compte x = new Compte(a, b, c, d);
    x.afficherResume();

    System.out.print("Donner le montant a deposer: "); m = sc.nextInt();
    x.crediter(m);
    x.afficherResume();

    System.out.print("Donner le montant a retirer: "); m = sc.nextInt();
    x.debiter(m);
    x.afficherResume();
    
    sc.nextLine(); // to clean the input
    
    System.out.println("Compte 2:");
    System.out.print("Donner Le CIN: "); a = sc.nextLine();
    System.out.print("Donner Le Nom: "); b = sc.nextLine();
    System.out.print("Donner Le Prenom: "); c = sc.nextLine();
    System.out.print("Donner Le numero de telephone: "); d = sc.nextLine();
    Compte y = new Compte(a, b, c, d);
    y.afficherResume();

    System.out.println("Crediter le compte 2 a partir du compte 1");
    System.out.print("Donner le montant a deposer: "); m = sc.nextInt();
    y.crediter(m, x);

    System.out.println("Debiter le compte 1 et crediter le compte 2");
    System.out.print("Donner le montant a retirer: "); m = sc.nextInt();
    x.debiter(m, y);

    x.afficherResume();
    y.afficherResume();
    y.afficherNombreComptes();
  }
}
