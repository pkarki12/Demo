/*
 * Parallel Programming
 * Assignment 2
 * 
 * Usage: java CoinFlip #threads #iterations
 * 
 * Prasiddha Karki
 */

import java.util.Random;
import java.lang.*;

class CoinFlip implements Runnable {

    int thread_id;
    int h; // holds number of heads for individual thread
    static int n; // holds total number of flips for individual thread
    static int heads = 0; // total number of heads
    static int num; // total number of flips
    Random r = new Random();
    
    CoinFlip(int id) {
	this.thread_id = id;
	h = 0; // resets the head count for new thread
    }

    public void run() {
	for (int i = 0; i < n; i++) {
	    if (r.nextBoolean()) { // takes True as heads
		h++;
	    }
	}
	heads += h; // Increments total heads by indiviual thread heads
    }
    
    public static void main(String[] args) {
	// Takes command line arguments with simple error handling
        if (args.length != 2) {
	    System.out.println("Usage: CoinFlip #threads #iterations");
	    return;
	}
	int t;
	try {
	    t = Integer.parseInt(args[0]);
	    num = Integer.parseInt(args[1]);
	} catch (NumberFormatException nfe) {
	    System.out.println("Usage: CoinFlip #threads #iterations");
	    return;
	}
	
	n = num / t;

	// Create and start thread objects
	Thread[] threads = new Thread[t];
	long start = System.currentTimeMillis();
	for (int i = 0; i < t; i++) {
	    threads[i] = new Thread(new CoinFlip(i));
	    threads[i].start();
	}
	// Await completion of all threads
	for ( int i = 0; i < t; i++ ) {
	    try {
		threads[i].join();
	    } catch (InterruptedException e) {
		System.out.println("Thread interrupted. Exception: " + e.toString() + " Message: " + e.getMessage()) ;
		return;
	    }
	}
	long end = System.currentTimeMillis();
	
	long time = end - start;
	System.out.printf("%d heads in %d coin tosses\n", heads, num);
	System.out.printf("Elapsed time: %dms\n", time);
    }
}
