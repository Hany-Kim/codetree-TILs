import java.util.*;

public class Main {
    public static void main(String[] args) {
        // Please write your code here.

        Scanner sc = new Scanner(System.in);

        String str = sc.next();

        String s = "";
        for(int i=0; i<8; i++){
            s += str;
        }

        System.out.println(s);
    }
}