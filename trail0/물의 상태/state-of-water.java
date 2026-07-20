import java.util.*;

public class Main {
    public static void main(String[] args) {
        // Please write your code here.

        Scanner sc = new Scanner(System.in);

        int n = sc.nextInt();

        if(n < 0){
            System.out.println("ice");
        } else if (n >= 100){
            System.out.println("vapor");
        } else {
            System.out.println("water");
        }

    }
}