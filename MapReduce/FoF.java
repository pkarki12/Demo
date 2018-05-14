import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;


public class FoF {
	// Mapper Class:
	public static class FoFMapper
			extends Mapper<Object, Text, Text, IntWritable> {

		// Value is just one
		private final static IntWritable one = new IntWritable(1);
		private Text key = new Text();
		public void map(Object k, Text v, Context c)
				throws IOException, InterruptedException {

			String[] line = v.toString().split(" ");
			// Iterate through the friends
			for (int i = 1; i < line.length; i++) {
				for (int j = 1; j < line.length; j++) {
					if (i != j) {
						key.set(line[0] + " " + line[i] + " " + line[j]);
						c.write(key, one);
						key.set(line[i] + " " + line[0] + " " + line[j]);  // Checks if friends are mutual
						c.write(key, one);
					}
				}
			}
		}
	}

	// Reducer Class:
	public static class FoFReducer
			extends Reducer<Text, IntWritable, Text, IntWritable> {

		public void reduce(Text key, Iterable<IntWritable> v,
				Context c) throws IOException, InterruptedException {

			int sum = 0;
			for (IntWritable val : v) {
				sum += val.get();
			}
			// Checks if friends are mutual
			if (sum > 1) {
				String[] line = key.toString().split(" ");
				if (Integer.parseInt(line[1]) < Integer.parseInt(line[2])) {  // Sorts friends
						c.write(key, null);  // Value does not matter
				}
			}
		}
	}

	public static void main(String[] args) throws Exception {
		Configuration conf = new Configuration();
		Job job = Job.getInstance(conf, "FoF");
		job.setJarByClass(FoF.class);
		job.setMapperClass(FoFMapper.class);
		job.setReducerClass(FoFReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(IntWritable.class);
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}