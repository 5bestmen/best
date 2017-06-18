package com.rbac.service;

import org.springframework.stereotype.Service;

import javax.servlet.http.HttpServletResponse;
import java.io.*;
import java.net.URLEncoder;

/**
 * Created by libh on 2015/12/28.
 */
@Service("chromeService")
public class ChromeService {
    public void downLoad(String path, HttpServletResponse response) {
        if (path == null)
            return;
        try {
//            String srcFile = System.getenv("TEMP") + File.separator + script.getID() + ".sar";
//                response.setHeader("content-disposition", "attachment;filename=" + URLEncoder.encode(script.getName() + ".sar", "UTF-8"));
            String srcFile = path + "ChromeStandaloneSetup.exe";
                response.setHeader("content-disposition", "attachment;filename=" + URLEncoder.encode("ChromeStandaloneSetup.exe", "UTF-8"));

            // 读取要下载的文件，保存到文件输入流
            FileInputStream in = null;

            in = new FileInputStream(srcFile);

            // 创建输出流
            OutputStream out = response.getOutputStream();
            // 创建缓冲区
            byte buffer[] = new byte[1024];
            int len;
            while ((len = in.read(buffer)) > 0) {
                out.write(buffer, 0, len);
            }
            in.close();
            out.close();

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
