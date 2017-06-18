package com.corsair.api;

import com.corsair.service.GetTreeDataService;
import net.sf.json.JSONArray;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;


/**
 * Created by zyj on 2017/2/6.
 */
@RestController
@RequestMapping(value = "/psr")
public class GetTreeDataController {

    @Autowired
    GetTreeDataService getTreeDataService;

    @RequestMapping(value = "/getTreeData", method = RequestMethod.GET)
    public ResponseEntity<String> getTreeDataEvent(){

        String strTreeData = getTreeDataService.getTreeDataEvent();
        JSONArray jsonArray = JSONArray.fromObject(strTreeData);
        return new ResponseEntity<>(jsonArray.toString(), HttpStatus.OK);
    }
}
