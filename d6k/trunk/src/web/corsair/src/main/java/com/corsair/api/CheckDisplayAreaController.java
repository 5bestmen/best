package com.corsair.api;

import com.corsair.service.DisplayAreaService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Created by zjq on 2016/1/27.
 */
@RestController
@RequestMapping(value = "/displayArea")
public class CheckDisplayAreaController {
    @Autowired
    DisplayAreaService displayAreaService;

    @RequestMapping(value = "checkDisplayAreaComments", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> checkDisplayAreaComments(String comments, HttpServletRequest request, HttpServletResponse response) {

        String ret = null;

        if (CommonUtils.isNotBlank(comments)) {
            Boolean isExist = displayAreaService.checkCommentsExist(comments);
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

        return new ResponseEntity<>(ret, HttpStatus.OK);
    }

    @RequestMapping(value = "checkDisplayAreaName", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> checkDisplayAreaName(String name, HttpServletRequest request, HttpServletResponse response) {

        String ret = null;

        if (CommonUtils.isNotBlank(name)) {
            Boolean isExist = displayAreaService.checkNameExist(name);
            if (isExist) {
                ret = "1";
            } else {
                ret = "0";
            }
        }

        return new ResponseEntity<>(ret, HttpStatus.OK);
    }
}
