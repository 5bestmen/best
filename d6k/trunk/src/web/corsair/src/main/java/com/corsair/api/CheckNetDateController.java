package com.corsair.api;

import com.corsair.service.ComplexBenefitService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;

/**
 * Created by zjq on 2016/1/27.
 */
@RestController
public class CheckNetDateController {
    @Autowired
    ComplexBenefitService complexBenefitService;
    @RequestMapping(value = "/checkNetDate", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> checkNetDate(String netDate, HttpServletRequest request, HttpServletResponse response){

        String ret = null;
        try {
            String dataNetDate = URLDecoder.decode(netDate, "UTF-8");

            if(CommonUtils.isNotBlank(dataNetDate)){
                Boolean isExist = complexBenefitService.checkExistNetDate(dataNetDate);
                response.setContentType("text/html;charset=UTF-8");
                response.setBufferSize(1024);
                response.setHeader("Pragma", "No-cache");
                response.setHeader("Cache-Control", "no-cache");
                if (isExist) {
                    ret = "1";
                } else {
                    ret = "0";
                }
            }
        }catch (UnsupportedEncodingException e){
            e.printStackTrace();
        }
        return new ResponseEntity<String>(ret, HttpStatus.OK);
    }
}
