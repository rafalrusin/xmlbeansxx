import org.apache.xmlbeans.*;
import com.p4.mind.mytest.*;
import java.io.*;
import org.apache.commons.logging.*;

public class speed01 {
    private static Log LOG = LogFactory.getLog(speed01.class);
    private static double currentTime() {
        return System.currentTimeMillis() / 1000.0;
    }

    public static String istreamToString(InputStream in) throws Exception {
        StringWriter out = new StringWriter();
        int c;

        while ((c = in.read()) != -1)
           out.write(c);

        in.close();
        out.close();
        return out.toString();
    }

    public static void main(String[] args) throws Exception {
        //Parsing speed test
        try {
            String in = istreamToString(new FileInputStream("d.xml"));
            while (true) {
                double t1 = currentTime();
                ContentDocument doc = ContentDocument.Factory.parse(in);
                doc.validate();
                
                double t2 = currentTime();
                
                int car = 0;
                for (int i = 0; i < 10; i++) {
                    car = ((Fullpersoninfo)(doc.getContent().getEmployee())).getCar();
                    ((Fullpersoninfo)(doc.getContent().getEmployee())).setCar(car + 1);
                }

                {
                    TreeType tree = ((Fullpersoninfo)(doc.getContent().getEmployee())).getTree().getLeft().getLeft();
                    ((Fullpersoninfo)(doc.getContent().getEmployee())).getTree().getRight().getLeft().setRight(tree);
                }
                
                double t3 = currentTime();
                
                String xml;
                {
                    StringWriter out = new StringWriter();
                    doc.save(out, new XmlOptions().setSavePrettyPrintIndent(0));
                    xml = out.toString();
                }
                double t4 = currentTime();

                LOG.info("Parse duration: " + (t2-t1) + " sec. " + (1. / (t2-t1)) + " per second.");
                LOG.info("Modify duration: " + (t3-t2) + " sec. " + (1. / (t3-t2)) + " per second.");
                LOG.info("Serialize duration: " + (t4-t3) + " sec. " + (1. / (t4-t3)) + " per second.");
                LOG.info("Car: " + car);
                //LOG.info("Xml: " + xml);
            }
        } catch (Exception e) {
            LOG.error("Exception: " + e.getMessage());
            throw e;
        }
    }
}

