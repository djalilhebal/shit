// Numerically-Keyed Columnar Transposition Cipher (Wikipedia it -.-)
// Not sure when I wrote this: 2018-02? -03? -04?
// Anyway, it was once we started "learning" Java at "school"...
// #unfinished, #abandoned

import java.util.Scanner;

public class NKTCipher {
	public static void print(String str) { System.out.print(str); }
	public static void println(String str) { System.out.println(str); }

	public static boolean VERBOSE = true;

	public static void orderTables(char[] characters, int[] positions) {
		// kichghl "Selection Sort"
		int i, j, k, min_index;
		char min_value, c;
		
		for (i = 0; i < characters.length; i++) {
			min_index = i;
			min_value = characters[i];
			for (j = i; j < characters.length; j++) {
				if (characters[j] < min_value) {
					min_index = j;
					min_value = characters[j];
				}
			}

			// now swap 'characters'
			c = characters[i];
			characters[i] = characters[min_index];
			characters[min_index] = c;
			// and their corresponding 'positions'
			k = positions[i];
			positions[i] = positions[min_index];
			positions[min_index] = k;
		};
	
	}
	
	public static int[] getPositions(String key) {
		int i, LEN = key.length();
		char[] characters = new char[LEN];
		int[] positions = new int[LEN];
		for (i = 0; i < LEN; i++) {
			positions[i] = i;
			characters[i] = key.charAt(i);
		}
		// before~ {O, R, A, N, G, E} and {0, 1, 2, 3, 4, 5}
		orderTables(characters, positions);
		// after~~ {A, E, G, N, O, R} and {2, 5, 4, 3, 0, 1}
		return positions;
	}
	
	public static String encode(String text, String key) {
		int i, j, t;
		int COLS = key.length();
		int ROWS = (int)Math.ceil( text.length() / key.length())+1;
		
		int[] positions = getPositions(key);
		
		// create and fill the matrix
		char[][] matrix = new char[ROWS][COLS];
		t = 0;
		for (i = 0; i < ROWS; i++) {
			for (j = 0; j < COLS; j++) {
				if (t < text.length()) {
					matrix[i][j] = text.charAt(t);
					t++;
				} else {
					// padding
					matrix[i][j] = 'K';
				}
			}
		}
		
		// print the matrix, for debugging
		for (i = 0; i < ROWS; i++) {
			for (j = 0; j < COLS; j++) {
				print(" " + matrix[i][j]);
			}
			print("\n");
		}
		
		String result = "";
		for (t = 0; t < positions.length; t++) {
			j = positions[t];
			for (i = 0; i < ROWS; i++) {
				result += matrix[i][j];
			}
		}
		
		return result;
	}
		
	public static void main(String args[])	{
		/*
		Scanner sc = new Scanner(System.in);
		print("Enter a message: "); String msg = sc.nextLine(); 
		print("Enter a key: "); String key = sc.nextLine();
		*/
		String msg = "request reinforcements immediately";
		String key = "orange";
		
		msg = msg.toUpperCase().replaceAll("[^A-Z]", "");
		key = key.toUpperCase().replaceAll("[^A-Z]", "");
		println("After cleaning:");
		println("msg: " + msg);
		println("key: " + key);
		
		println("Ciphertext: " + encode(msg, key));
	}
}
