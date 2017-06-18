package com.corsair.api;

import com.corsair.device.MaterialPSR;
import com.corsair.service.MaterialService;
import com.rbac.util.CommonUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by zjq on 2016/1/26.
 */
@RestController
@RequestMapping(value = "/material")
public class MaterialController {
    @Autowired
    MaterialService materialService;

    @RequestMapping(value = "getMaterialPsrList", method = RequestMethod.POST)
    public ResponseEntity<List<MaterialPSR>> getMaterialPsrList(String dataID, String dateType, String dataDesc, String dataExtendDesc, String queryFlag) {


        List<MaterialPSR> materialPSRList = new ArrayList<>();
        if (queryFlag.equals("false")){
            if (dateType.equals("materialType")){
                materialPSRList = materialService.getMaterialModelList(dataID);
            }
            else if (dateType.equals("materialModel")){
                materialPSRList =  materialService.getMaterialList(dataID);
            }
        }
        else {
            if (CommonUtils.isBlank(dataDesc) || CommonUtils.isBlank(dataExtendDesc)){
                materialPSRList = materialService.getQueryMaterialList(dataDesc,dataExtendDesc);
            }
        }

        return new ResponseEntity<>(materialPSRList, HttpStatus.OK);
    }


    @RequestMapping(value = "checkMaterialName", method = RequestMethod.POST, produces = "text/html;charset=UTF-8")
    public ResponseEntity<String> CheckMaterialName(String mType,String materialName, HttpServletRequest request, HttpServletResponse response){

        String ret = null;

        Boolean isExist = false;
        if(mType.equals("materialType")){
            isExist = materialService.checkExistmaterialTypeName(materialName);
        }
        else if(mType.equals("materialModel")){
            isExist = materialService.checkExistMaterialModelCodeId(materialName);
        }
        else if(mType.equals("material")){
            isExist = materialService.checkExistMaterialFixedId(materialName);
        }

        if (isExist) {
            ret = "1";
        } else {
            ret = "0";
        }

        return new ResponseEntity<>(ret, HttpStatus.OK);
    }

}
