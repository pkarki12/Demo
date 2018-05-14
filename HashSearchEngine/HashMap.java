import java.util.Iterator;
import java.util.ArrayList;

/**
 * Implementation of Map that involves hashing. It makes use of the
 * separate chaining technique. Once the hashtable is filled to a
 * certain size determined by the load factor, the hash table
 * doubles in size.
 *
 * Prasiddha Karki    pkarki1@jhu.edu
 *
 * @param <K> Type for keys.
 * @param <V> Type for values.
 */
public class HashMap<K, V> implements Map<K, V> {
    static final int STARTING_CAPACITY = 10;
    static final int LOAD_FACTOR = 7; // Range from 1-10.

    private class Node {
        Node next;
        Node prev;
        K key;
        V value;

        Node(K k, V v) {
            this.key = k;
            this.value = v;
            this.next = null;
            this.prev = null;
        }
    }

    private ArrayList<Node> table;
    private int capacity;
    private int size;

    /**
     * Default Constructor.
     */
    public HashMap() {
        this.table = new ArrayList<>();
        this.capacity = STARTING_CAPACITY;
        this.size = 0;
        for (int i = 0; i < this.capacity; i++) {
            this.table.add(null);
        }
    }

    @Override
    public int size() {
        return this.size;
    }

    // Returns node for given key.
    private Node find(K k) {
        if (k == null) {
            throw new IllegalArgumentException("cannot handle null key");
        }
        Node n = this.table.get(this.getIndex(k));
        while (n != null) {
            if (n.key.equals(k)) {
                return n;
            }
            n = n.next;
        }
        return null;
    }

    @Override
    public boolean has(K k) {
        if (k == null) {
            return false;
        }
        return this.find(k) != null;
    }

    // Return node for given key, throw an exception if the key is not
    // in the tree.
    private Node findForSure(K k) {
        Node n = this.find(k);
        if (n == null) {
            throw new IllegalArgumentException("cannot find key " + k);
        }
        return n;
    }

    // Returns index for given key using hash function
    private int getIndex(K k) {
        return Math.abs(k.hashCode() % this.capacity);
    }

    @Override
    public void put(K k, V v) {
        this.findForSure(k).value = v;
    }

    @Override
    public V get(K k) {
        return this.findForSure(k).value;
    }

    // Doubles size of hash table if certain portion of the
    // table is filled.
    private void grow() {
        ArrayList<Node> temp = this.table;
        this.table = new ArrayList<>();
        this.capacity = 2 * this.capacity;
        this.size = 0;
        for (int i = 0; i < this.capacity; i++) {
            this.table.add(null);
        }
        for (Node n : temp) {
            Node head = n;
            while (head != null) {
                this.insert(head.key, head.value);
                head = head.next;
            }
        }
    }

    @Override
    public void insert(K k, V v) {
        if (this.has(k)) {
            throw new IllegalArgumentException("duplicate key " + k);
        }
        int index = this.getIndex(k);
        Node n = new Node(k, v);
        Node head = this.table.get(index);
        if (head != null) {
            n.next = head;
            head.prev = n;
        }
        this.table.set(index, n);
        this.size++;
        if (((10 * this.size) / this.capacity) >= LOAD_FACTOR) {
            this.grow();
        }
    }

    @Override
    public V remove(K k) {
        if (!this.has(k)) {
            return null;
        }
        Node n = this.findForSure(k);
        if (n.next != null) {
            n.next.prev = n.prev;
        }
        if (n.prev != null) {
            n.prev.next = n.next;
        }
        this.size--;
        n.next = null;
        n.prev = null;
        return n.value;
    }

    @Override
    public Iterator<K> iterator() {
        ArrayList<K> keys = new ArrayList<>();
        for (Node n : this.table) {
            Node head = n;
            while (head != null) {
                keys.add(head.key);
                head = head.next;
            }
        }
        return keys.iterator();
    }

    /**
     * Builds string representation for hashtable.
     * @return String string hashtable
     */
    public String toString() {
        StringBuilder s = new StringBuilder();
        ArrayList<Node> a = new ArrayList<Node>();
        for (Node n : this.table) {
            Node head = n;
            while (head != null) {
                a.add(head);
                head = head.next;
            }
        }
        s.append("{");
        for (int i = 0; i < a.size(); i++) {
            s.append(a.get(i).key);
            s.append(": ");
            s.append(a.get(i).value);
            if (i != a.size() - 1) {
                s.append(", ");
            }
        }
        s.append("}");
        return s.toString();
    }
}
