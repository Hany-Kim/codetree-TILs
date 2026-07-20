import java.util.*;

public class Main {
    public static void main(String[] args) {
        // Please write your code here.
        Scanner sc = new Scanner(System.in);

        int n = sc.nextInt();

        if (n >= 3000){
            System.out.println("book");
        } else if (n >= 1000){
            System.out.println("mask");
        } else {
            System.out.println("no");
        }
    }
}