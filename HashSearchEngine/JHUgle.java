import java.util.Scanner;
import java.util.ArrayList;
import java.util.Stack;
import java.util.EmptyStackException;
import java.io.FileNotFoundException;
import java.io.File;

/**
 * Search engine implementation using hash table.
 *
 * Prasiddha Karki    pkarki1@jhu.edu
 */
public final class JHUgle {
    private static Map<String, ArrayList<String>> data;

    // Make checkstyle happy.
    private JHUgle() {}

    /**
     *  Main method.
     *  @param args Command line arguments (ignored).
     */
    public static void main(String[] args) {
        ArrayList<String> keys = new ArrayList<>();
        int flag = 0;
        try {
            File infile = new File(args[0]);
            data = new HashMap<String, ArrayList<String>>();
            Scanner scanner = new Scanner(infile);

            while (scanner.hasNextLine()) {
                String s = scanner.nextLine();
                String s2 = scanner.nextLine();
                ArrayList<String> words = new ArrayList<String>();
                for (String word : s2.split(" ")) {
                    words.add(word);
                }
                data.insert(s, words);
                keys.add(s);
            }
        } catch (FileNotFoundException e) {
            System.out.println("File not found");
        }
        System.out.println("Index Created");
        Stack<String> st = new Stack<>();
        Scanner scanner2 = new Scanner(System.in);
        while (scanner2.hasNext()) {
            String s = scanner2.next();
            if (("!").equals(s)) {
                System.exit(1);
            } else if (("?").equals(s)) {
                try {
                    if (flag == 0) {
                        String top = st.peek();
                        for (String key : keys) {
                            ArrayList<String> values = data.get(key);
                            if (values.contains(top)) {
                                System.out.println(key);
                                continue;
                            }
                        }
                    } else if (flag == 1) {
                        String top = st.pop();
                        String nextone = st.peek();
                        st.push(top);
                        for (String key : keys) {
                            ArrayList<String> values = data.get(key);
                            if (values.contains(top)
                                && values.contains(nextone)) {
                                System.out.println(key);
                                continue;
                            }
                        }
                        flag = 0;
                    } else if (flag == 2) {
                        String top = st.pop();
                        String nextone = st.peek();
                        st.push(top);
                        for (String key : keys) {
                            ArrayList<String> values = data.get(key);
                            if (values.contains(top)
                                || values.contains(nextone)) {
                                System.out.println(key);
                                continue;
                            }
                        }
                        flag = 0;
                    }
                } catch (EmptyStackException ese) {
                    System.err.println("Empty Stack");
                }
            } else if (("&&").equals(s)) {
                flag = 1;
            } else if (("||").equals(s)) {
                flag = 2;
            } else {
                st.push(s);
            }
        }
    }
}
