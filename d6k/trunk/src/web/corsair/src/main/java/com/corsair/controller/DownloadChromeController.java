package com.corsair.controller;

import com.corsair.form.DownloadChromeForm;
import com.rbac.service.ChromeService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.File;

/**
 * Created by libh on 2015/12/29.
 */
@Controller
public class DownloadChromeController {
    @Autowired
    ChromeService chromeService;
    @RequestMapping(value = "/downloadChrome", method = RequestMethod.GET)
    public ModelAndView redirect(DownloadChromeForm downloadChromeFor, HttpServletRequest request, HttpServletResponse response){
        ModelAndView modelAndView = new ModelAndView("downloadChrome");

        return modelAndView;
    }

    @RequestMapping(value = "/downloadChrome", method = RequestMethod.POST)
    public ModelAndView redirectChrome(DownloadChromeForm downloadChromeForm, HttpServletRequest request, HttpServletResponse response){
        ModelAndView modelAndView = new ModelAndView("downloadChrome");

        String fileUploadPath = request.getSession().getServletContext().getRealPath("/") + "resources" +  File.separator + "file" + File.separator;
        chromeService.downLoad(fileUploadPath, response);
        return modelAndView;
    }
}
