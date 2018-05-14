import org.junit.Before;
import org.junit.Test;
import java.util.Iterator;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;

/**
 * Testing implementations of the Map interface, which can be applied
 * to all implementations of the Map interface.
 *
 * @author Prasiddha Karki    pkarki1@jhu.edu
 */
public abstract class MapTestBase {
    Map<Integer, String> m;

    abstract Map<Integer, String> createMap();

    @Before
    public void setupMapTests() {
        m = this.createMap();
    }

    @Test
    public void newSize() {
        assertEquals(m.size(), 0);
    }

    @Test
    public void insertRemove() {
        m.insert(1, "A");
        m.insert(2, "B");
        m.insert(3, "C");
        assertEquals(m.remove(1), "A");
        assertEquals(m.remove(2), "B");
        assertEquals(m.remove(3), "C");
    }

    @Test(expected=IllegalArgumentException.class)
    public void insertRedundantKey() {
        m.insert(1, "A");
        m.insert(1, "B");
    }

    @Test
    public void insertPut() {
        m.insert(1, "A");
        m.put(1, "B");
        assertEquals(m.remove(1), "B");
    }

    @Test(expected=IllegalArgumentException.class)
    public void putFakeKey() {
        m.put(1, "A");
    }

    @Test
    public void insertGet() {
        m.insert(1, "A");
        m.insert(2, "B");
        m.insert(3, "C");
        assertEquals(m.get(1), "A");
        assertEquals(m.get(2), "B");
        assertEquals(m.get(3), "C");
    }

    @Test(expected=IllegalArgumentException.class)
    public void getFakeKey() {
        m.get(1);
    }

    @Test
    public void insertHas() {
        m.insert(1, "A");
        assertTrue(m.has(1));
    }

    @Test
    public void hasFakeKey() {
        assertFalse(m.has(1));
    }

    @Test
    public void sizeWorks() {
        m.insert(1, "A");
        m.insert(2, "B");
        m.insert(3, "C");
        assertEquals(m.size(), 3);
        m.remove(2);
        assertEquals(m.size(), 2);
    }

    @Test
    public void iterator() {
        m.insert(1, "A");
        m.insert(2, "B");
        m.insert(3, "C");
        Iterator<Integer> i = m.iterator();
    }
}
