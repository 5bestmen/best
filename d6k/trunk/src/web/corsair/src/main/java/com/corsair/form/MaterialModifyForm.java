package com.corsair.form;

/**
 * Created by libh on 2015/10/27.
 */
public class MaterialModifyForm{
    private String submit;

    private String id;

    private String editId;

    private String nodeId;

    private String mType;

    private String codeId;

    private String materialModelLables;

    private String fixedId;

    private String materialExtendLables;

    private String materialTypeId;

    private String materialModelId;

    private String addModelLables;

    private String addExtendLables;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getEditId() {
        return editId;
    }

    public void setEditId(String editId) {
        this.editId = editId;
    }

    public String getNodeId() {
        return nodeId;
    }

    public void setNodeId(String nodeId) {
        this.nodeId = nodeId;
    }

    public String getmType() {
        return mType;
    }

    public void setmType(String mType) {
        this.mType = mType;
    }

    public String getSubmit() {
        return submit;
    }

    public void setSubmit(String submit) {
        this.submit = submit;
    }

    public String getCodeId() {
        return codeId;
    }

    public void setCodeId(String codeId) {
        this.codeId = codeId;
    }

    public String getFixedId() {
        return fixedId;
    }

    public void setFixedId(String fixedId) {
        this.fixedId = fixedId;
    }

    public String getMaterialTypeId() {return materialTypeId;}

    public void setMaterialTypeId(String materialTypeId) {
        this.materialTypeId = materialTypeId;
    }

    public String getMaterialModelId() {return materialModelId;}

    public void setMaterialModelId(String materialModelId) {
        this.materialModelId = materialModelId;
    }

    public String getMaterialModelLables() {
        return materialModelLables;
    }

    public void setMaterialModelLables(String materialModelLables) {
        this.materialModelLables = materialModelLables;
    }

    public String getMaterialExtendLables() {
        return materialExtendLables;
    }

    public void setMaterialExtendLables(String materialExtendLables) { this.materialExtendLables = materialExtendLables; }

    public String getAddModelLables(){return addModelLables;}

    public void setAddModelLables(String addModelLables){this.addModelLables = addModelLables;}

    public String getAddExtendLables(){return addExtendLables;}

    public void setAddExtendLables(String addExtendLables){this.addExtendLables = addExtendLables;}
//    /**
//     * Method validate
//     * @param mapping
//     * @param request
//     * @return ActionErrors
//     */
//    public ActionErrors validate(ActionMapping mapping,
//                                 HttpServletRequest request) {
//        // TODO Auto-generated method stub
//        return null;
//    }


}
