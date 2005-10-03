import org.apache.xmlbeans.*;
import com.p4.mind.mytest.*;
import java.io.*;
import org.apache.commons.logging.*;

public class speed01 {
    private static Log LOG = LogFactory.getLog(speed01.class);
    private static double currentTime() {
        return System.currentTimeMillis() / 1000.0;
    }

    public static void main(String[] args) throws Exception {
        //Parsing speed test
        try {
            while (true) {
                double t1 = currentTime();
                ContentDocument doc = ContentDocument.Factory.parse(new File("d.xml"));
                int car = ((Fullpersoninfo)(doc.getContent().getEmployee())).getCar();
                String xml = doc.toString();
                double t2 = currentTime();
                LOG.info("Duration: " + (t2-t1) + " sec. " + (1. / (t2-t1)) + " per second");
                LOG.debug("Car: " + car);
                LOG.debug("Xml: " + xml);
            }
        } catch (Exception e) {
            LOG.error("Exception: " + e.getMessage());
            throw e;
        }
    }
}

