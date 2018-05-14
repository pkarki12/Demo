import org.junit.Test;
import static org.junit.Assert.assertEquals;

/**
 * Instantiate the HashMap to test.
 */
public class HashMapTest extends MapTestBase {
    @Override
    protected Map<Integer, String> createMap() {
        return new HashMap<>();
    }

    @Test
    public void toStringWorks() {
        m.insert(1, "A");
        m.insert(2, "B");
        m.insert(3, "C");
        assertEquals(m.toString(), "{1: A, 2: B, 3: C}");
    }
}
