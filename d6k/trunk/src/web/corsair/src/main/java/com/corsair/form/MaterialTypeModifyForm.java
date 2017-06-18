package com.corsair.form;

/**
 * Created by libh on 2015/10/26.
 */
public class MaterialTypeModifyForm{
    private String submit;

    private String materialTypeName;

    private String materialTypeDesc;

    private String id;

    private String menuIds;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getMenuIds() {
        return menuIds;
    }

    public void setMenuIds(String menuIds) {
        this.menuIds = menuIds;
    }

    public String getMaterialTypeName() {
        return materialTypeName;
    }

    public void setMaterialTypeName(String materialTypeName) {
        this.materialTypeName = materialTypeName;
    }

    public String getMaterialTypeDesc() {
        return materialTypeDesc;
    }

    public void setMaterialTypeDesc(String materialTypeDesc) {
        this.materialTypeDesc = materialTypeDesc;
    }

    public String getSubmit() {
        return submit;
    }

    public void setSubmit(String submit) {
        this.submit = submit;
    }

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
