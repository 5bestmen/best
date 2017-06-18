package com.corsair.util;


import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.Properties;
/**
 * Created by Administrator on 2016/1/5.
 */
public class PropertiesUtils
{
    public static Properties loadPropertyFile(String fileName)
    {
        Properties prop = new Properties();
        InputStream in = null;
        try
        {
            in = Thread.currentThread().getContextClassLoader().getResourceAsStream(fileName);
            prop.load(in);
            in.close();
        }
        catch (Exception e)
        {

        }
        return prop;
    }

    public static String loadProperty(String key)
    {
        Properties prop = new Properties();
        InputStream in = null;
        String resultStr = "";
        try
        {
            in = Thread.currentThread().getContextClassLoader().getResourceAsStream("export.properties");
            prop.load(in);
            in.close();
            resultStr = prop.getProperty(key);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

        return resultStr;
    }

    public static String loadProperty(String fileName, String key)
    {
        Properties prop = new Properties();
        InputStream in = null;
        String resultStr = "";
        try
        {
            in = Thread.currentThread().getContextClassLoader().getResourceAsStream(fileName + ".properties");
            prop.load(in);
            in.close();
            resultStr = prop.getProperty(key);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

        return resultStr;
    }

    /**
     * 测试功能类
     *
     * @param args
     */
    public static void main(String[] args)
    {
        Properties pop = PropertiesUtils.loadPropertyFile("export.properties");
        //Enumeration enumKeys = pop.keys();
        //while (enumKeys.hasMoreElements())
        //{
        System.out.println(pop.getProperty("startTime"));
        System.out.println(pop.getProperty("endTime"));
        //}
    }
}
