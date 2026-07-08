import java.util.*;

public class Main {
    public static void main(String[] args) {
        // Please write your code here.

        Scanner sc = new Scanner(System.in);

        int n = sc.nextInt();

        if(n >= 80){
            System.out.println("pass");
        }
        else {
            int x = 80 - n;
            System.out.println(x + " more score");
        }
    }
}