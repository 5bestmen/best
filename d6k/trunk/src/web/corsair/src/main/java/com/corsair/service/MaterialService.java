package com.corsair.service;

import com.alibaba.fastjson.JSONArray;
import com.corsair.dao.*;

import com.corsair.device.MaterialPSR;
import com.corsair.entity.*;
import com.rbac.util.CommonUtils;
import net.sf.json.JSONException;
import net.sf.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * Created by libh on 2015/10/19.
 */
@Service("materialService")
public class MaterialService {
    @Autowired
    MaterialTypeDao materialTypeDao;
    @Autowired
    MaterialModelDao materialModelDao;
    @Autowired
    MaterialDao materialDao;
    @Autowired
    MaterialModelLableDao materialModelLableDao;
    @Autowired
    MaterialExtendLableDao materialExtendLableDao;

    //获取所有物资类别
    public List<MaterialType> getMaterialTypes(){ return materialTypeDao.getMaterialTypes();}

    //获取所有物资描述列表
    public List<MaterialModelLable> getAllModelLableList(){
        return materialModelLableDao.getMaterialModelLables();
    }

    //获取所有物资扩展描述列表
    public List<MaterialExtendLable> getAllExtendLableList(){
        return materialExtendLableDao.getMaterialExtendLables();
    }

    //获取物资树列表
    public String getMaterialTree(){
        List<MaterialType> materialTypeList = new ArrayList<>();
        materialTypeList = materialTypeDao.getMaterialTypes();
        List<MaterialPSR> levelOneList = new ArrayList<>();

        for(MaterialType materialType : materialTypeList){
            MaterialPSR levelOnePSR = new MaterialPSR();
            levelOnePSR.setId(materialType.getMaterialTypeID());
            levelOnePSR.setName(materialType.getName());
            levelOnePSR.setDesc(materialType.getComments());
            levelOnePSR.setType("");
            levelOnePSR.setArea("materialType");

            List<MaterialPSR> levelTwoList = getMaterialModelList(levelOnePSR.getId());
            for (MaterialPSR levelTwoPSR : levelTwoList){
                levelOnePSR.getChildren().add(levelTwoPSR);
                List<MaterialPSR> levelThereList = getMaterialList(levelTwoPSR.getId());
                for (MaterialPSR levelTherePSR : levelThereList){
                    levelTwoPSR.getChildren().add(levelTherePSR);
                }

            }
            levelOneList.add(levelOnePSR);
        }
        JSONArray array = new JSONArray();
        for (MaterialPSR materialPSR : levelOneList) {
            array.add(objToJson(materialPSR));
        }
        String ret = array.toString().replace("children", "nodes");
        return ret;
    }

    /**
     * 菜单实体转json对象
     * @param materialPSR
     * @return
     */
    private static JSONObject objToJson(MaterialPSR materialPSR) {
        JSONObject obj = new JSONObject();

        try {
            obj.put("id", materialPSR.getId());
            if (CommonUtils.isNotBlank(materialPSR.getDesc())){
                obj.put("text", materialPSR.getDesc());
            }
            else {
                obj.put("text", materialPSR.getType());
            }
            obj.put("area", materialPSR.getArea());
            if (materialPSR.getChildren().size() > 0) {
                JSONArray childArray = new JSONArray();
                for (MaterialPSR child : materialPSR.getChildren()) {
                    childArray.add(objToJson(child));
                }
                obj.put("children", childArray);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return obj;
    }

    //获取型号描述
    public String getMaterialDesc(MaterialModel materialModel){
        List<MaterialModelLable> materialModelLableList = new ArrayList<>();
        materialModelLableList.addAll(materialModel.getMaterialModelLables());
        List<String> stringList = new ArrayList<>();
        for(MaterialModelLable materialModelLable : materialModelLableList){
           stringList.add(materialModelLable.getName());
        }
        return listToString(stringList);
    }

    // 获取物料扩展描述
    public String getMaterialExtendDesc(Material material){
        List<MaterialExtendLable> materialExtendLableList = new ArrayList<>();
        materialExtendLableList.addAll(material.getMaterialExtendLables());
        List<String> stringList = new ArrayList<>();
        for (MaterialExtendLable materialExtendLable : materialExtendLableList){
            stringList.add(materialExtendLable.getName());
        }
        return listToString(stringList);
    }

    // List<String> to String
    public String listToString(List<String> stringList){
        if (stringList==null) {
            return null;
        }
        StringBuilder result=new StringBuilder();
        boolean flag=false;
        for (String string : stringList) {
            if (flag) {
                result.append(",");
            }else {
                flag=true;
            }
            result.append(string);
        }
        return result.toString();
    }

    /**
     * 根据指定物资类别获取物资materialmodel数组
     * @param id = materialType.materialTypeID
     * @return
     */
    public List<MaterialPSR> getMaterialModelList(String id){
        MaterialType materialType;
        if (id.equals("")){
            String str = getMaterialTypes().get(0).getMaterialTypeID();
            materialType = getMaterialTypeById(str);
        }else {
            materialType = getMaterialTypeById(id);
        }
        List<MaterialPSR> materialPSRList = new ArrayList<>();
        List<MaterialModel> materialModelList = new ArrayList<>();
        materialModelList.addAll(materialType.getMaterialModels());
        for (MaterialModel materialModel : materialModelList){
            MaterialPSR materialPSR = new MaterialPSR();
            materialPSR.setId(materialModel.getMaterialModelID());
            materialPSR.setName(materialModel.getCodeID());
            materialPSR.setDesc(getMaterialDesc(materialModel));
            materialPSR.setType(materialType.getComments());
            materialPSR.setArea("materialModel");
            materialPSRList.add(materialPSR);
        }
        return materialPSRList;
    }

    /**
     * 根据指定物资型号获取物资material数组
     * @param id = materialModel.materialModelID
     * @return
     */
    public List<MaterialPSR> getMaterialList(String id){
        if (id.equals("")){
            return null;
        }
        MaterialModel materialModel = getMaterialModelById(id);
        List<MaterialPSR> materialPSRList = new ArrayList<>();
        List<Material> materialList = new ArrayList<>();
        materialList.addAll(materialModel.getMaterials());
        for (Material material : materialList){
            MaterialPSR materialPSR = new MaterialPSR();
            materialPSR.setId(material.getMaterialID());
            materialPSR.setName(material.getFixedID());
            materialPSR.setDesc(getMaterialExtendDesc(material));
            materialPSR.setType(getMaterialDesc(materialModel));
            materialPSR.setArea("material");
            materialPSRList.add(materialPSR);
        }
        return materialPSRList;
    }

    /**
     * 查询：根据指定物料描述或扩展描述查询物资material数组
     * @param modelLabel,extendLabel
     * @return
     */
    public List<MaterialPSR> getQueryMaterialList(String modelLabel, String extendLabel){
        List<MaterialPSR> materialPSRList = new ArrayList<>();
        if (CommonUtils.isNotBlank(modelLabel)){
            List<MaterialModelLable> materialModelLableList = materialModelLableDao.getMaterialModelLabelList(modelLabel);
            for (MaterialModelLable materialModelLable : materialModelLableList){
                List<MaterialModel> materialModelList = new ArrayList<>();
                materialModelList.addAll(materialModelLable.getMaterialModels());
                for(MaterialModel materialModel : materialModelList){
                    MaterialPSR materialPSR = new MaterialPSR();
                    materialPSR.setId(materialModel.getMaterialModelID());
                    materialPSR.setName(materialModel.getCodeID());
                    materialPSR.setDesc(getMaterialDesc(materialModel));
                    materialPSR.setType(materialModel.getMaterialType().getComments());
                    materialPSR.setArea("materialModel");
                    materialPSRList.add(materialPSR);
                }
            }
        }
        if (CommonUtils.isNotBlank(extendLabel)){
            List<MaterialExtendLable> materialExtendLableList = materialExtendLableDao.getMaterialExtendLabelList(extendLabel);
            for (MaterialExtendLable materialExtendLable : materialExtendLableList){
                List<Material> materialList = new ArrayList<>();
                materialList.addAll(materialExtendLable.getMaterials());
                for (Material material : materialList){
                    MaterialPSR materialPSR = new MaterialPSR();
                    materialPSR.setId(material.getMaterialID());
                    materialPSR.setName(material.getFixedID());
                    materialPSR.setDesc(getMaterialExtendDesc(material));
                    materialPSR.setType(getMaterialDesc(material.getMaterialModel()));
                    materialPSR.setArea("material");
                    materialPSRList.add(materialPSR);
                }
            }
        }
        return materialPSRList;
    }


    /**
     * 检查物资类别id是否已经存在
     * @param name
     * @return
     */
    public boolean checkExistmaterialTypeName(String name){
        List list = materialTypeDao.getMaterialTypeListByExactName(name);
        if(list.size()>0){
            return true;
        }
        return false;
    }

    /**
     * 检查物资编码(ModelCodeID)是否已经存在
     * @param codeId
     * @return
     */
    public boolean checkExistMaterialModelCodeId(String codeId){
        List list = materialModelDao.getMaterialModelListByExactCodeId(codeId);
        if(list.size()>0){
            return true;
        }
        return false;
    }

    /**
     * 检查物资固定ID(fixedId)是否已经存在
     * @param fixedId
     * @return
     */
    public boolean checkExistMaterialFixedId(String fixedId){
        List list = materialDao.getMaterialModelListByExactFixedId(fixedId);
        if(list.size()>0){
            return true;
        }
        return false;
    }


    /**
     * 根据物资类别id查找物资类别
     * @param materialTypeID
     * @return
     */
    public MaterialType getMaterialTypeById(String materialTypeID){
        return materialTypeDao.findById(MaterialType.class, materialTypeID);
    }

    /**
     * 根据物资modelId查找物资model
     * @param materialModelID
     * @return
     */
    public MaterialModel getMaterialModelById(String materialModelID){
        return materialModelDao.findById(MaterialModel.class, materialModelID);
    }

    /**
     * 根据物资Id查找物资
     * @param materialID
     * @return
     */
    public Material getMaterialById(String materialID){
        return materialDao.findById(Material.class, materialID);
    }

    /**
     * 保存物资类别(materialType)实体
     * @param materialType
     */
    public void saveOrUpdateMaterialType(MaterialType materialType){
        materialTypeDao.saveOrUpdate(materialType);
    }

    /**
     * 删除物资类别(materialType)实体
     * @param id
     */
    public void deleteMaterialType(String id){
        MaterialType materialType = this.getMaterialTypeById(id);
        materialTypeDao.delete(materialType);
    }

    /**
     * 保存物资型号(materialModel)实体
     * @param materialModel
     */
    public void saveOrUpdateMaterialModel(MaterialModel materialModel){
        materialModelDao.saveOrUpdate(materialModel);
        String[] modelLableList = materialModel.getModelLables().split(",");
        List<String> modelLabelDescs = new ArrayList<>();
        List<MaterialModelLable> materialModelLableList = getAllModelLableList();
        for (MaterialModelLable model : materialModelLableList){
            modelLabelDescs.add(model.getName());
        }
        Set<MaterialModelLable> materialModelLables = new HashSet<MaterialModelLable>();
        for(String label : modelLableList){
            MaterialModelLable materialModelLable = new MaterialModelLable();
            if (modelLabelDescs.contains(label)){
                materialModelLable = materialModelLableDao.getMaterialModelLabel(label);
            }
            materialModelLable.setName(label);
            materialModelLableDao.saveOrUpdate(materialModelLable);
            materialModelLables.add(materialModelLable);
        }
        materialModel.setMaterialModelLables(materialModelLables);
    }

    /**
     * 删除物资型号(materialModel)实体
     * @param id
     */
    public void deleteMaterialModel(String id){
        MaterialModel materialModel = this.getMaterialModelById(id);
        materialModelDao.delete(materialModel);
    }

    /**
     * 保存物资(material)实体
     * @param material
     */
    public void saveOrUpdateMaterial(Material material ){
        materialDao.saveOrUpdate(material);

        Set<MaterialExtendLable> materialExtendLables = new HashSet<MaterialExtendLable>();
        if (material.getExtendLabels().equals("") || material.getExtendLabels().length() == 0){
            materialExtendLables = null;
            material.setMaterialExtendLables(materialExtendLables);
            return;
        }
        String[] extendLableList = material.getExtendLabels().split(",");
        List<String> extendLabelDescs = new ArrayList<>();
        List<MaterialExtendLable> materialExtendLableList = getAllExtendLableList();
        for (MaterialExtendLable extendLable : materialExtendLableList){
            extendLabelDescs.add(extendLable.getName());
        }

        for(String label : extendLableList){
            MaterialExtendLable materialExtendLable = new MaterialExtendLable();
            if (extendLabelDescs.contains(label)){
                materialExtendLable = materialExtendLableDao.getMaterialExtendLabel(label);
            }
            materialExtendLable.setName(label);
            materialExtendLableDao.saveOrUpdate(materialExtendLable);

            materialExtendLables.add(materialExtendLable);
        }
        material.setMaterialExtendLables(materialExtendLables);
    }

    /**
     * 删除物资(material)实体
     * @param id
     */
    public void deleteMaterial(String id){
        Material material = this.getMaterialById(id);
        materialDao.delete(material);
    }

}
