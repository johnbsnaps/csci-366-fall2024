package edu.montana.csci.csci366.examples;

import javax.sound.midi.Soundbank;
import java.util.HashMap;
import java.util.Scanner;

public class Perf {

    static HashMap<Integer, Long> CACHE = new HashMap<>();
    private static long fib(int n) {
        if (n < 1) {
            return 1;
        } else {
            Long cachedResult = CACHE.get(n);
            if (cachedResult != null) {
                return cachedResult;
            } else {
                //System.out.println("Calculating fib for " + n);
                long result = fib(n - 2) + fib(n - 1);
                CACHE.put(n, result);
                return result;
            }
        }
    }

    public static void main(String[] args) {
        System.out.println("Enter a number:");
        Scanner sc = new Scanner(System.in);
        int number = sc.nextInt();
        long start = System.currentTimeMillis();
        long fib = fib(number);
        long end = System.currentTimeMillis();
        System.out.println("Fib of " + number + " is " + fib + " (Calculated in " + (end - start) + " milliseconds)");
    }

}
