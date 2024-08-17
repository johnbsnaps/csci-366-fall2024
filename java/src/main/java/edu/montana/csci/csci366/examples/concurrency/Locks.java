package edu.montana.csci.csci366.examples.concurrency;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Locks {

    static Lock lock = new ReentrantLock();

    static int shared = 0;
    

    public static boolean func1 () {
        boolean x = Math.random() > .5, y = Math.random() > .5;
        return x && y;
    }

    public static boolean func2 () {
        boolean x = Math.random() > .5, y = Math.random() > .5;
        if(!x) {
            return false;
        }
        if(!y) {
            return false;
        }
        return true;
    }


    public static void run() {
        while(true){
            lock.lock();
            int initial = shared;
            safeSleep(1000);
            shared = initial + 1;
            System.out.println("Thread " +
                    Thread.currentThread().getId() +
                    ": Running... " + initial +
                    " plus 1 is " + shared);
            lock.unlock();
            safeSleep(1000);
        }
    }

    public static void main(String[] args) {
        Thread thread1 = new Thread(Locks::run);
        Thread thread2 = new Thread(Locks::run);
        thread1.start();
        thread2.start();
    }

    protected static void safeSleep(int l) {
        try {
            Thread.sleep(l);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

}
