package com.corsair.form;

/**
 * Created by libh on 2015/10/22.
 */
public class MaterialForm{

    private String id;

    private String describe;

    private String extendDesc;

    private String type;

    private String query;

    private String del;

    private String deleteIds;

    public String getDel() {
        return del;
    }

    public void setDel(String del) {
        this.del = del;
    }

    public String getDeleteIds() {return deleteIds;}

    public void setDeleteIds(String deleteIds) {
        this.deleteIds = deleteIds;
    }

    public String getQuery() {
        return query;
    }

    public void setQuery(String query) {
        this.query = query;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getDescribe() {return describe; }

    public void setDescribe(String describe) {this.describe = describe; }

    public String getExtendDesc() {return extendDesc; }

    public void setExtendDesc(String extendDesc) {this.extendDesc = extendDesc; }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
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
//
//    /**
//     * Method reset
//     * @param mapping
//     * @param request
//     */
//    public void reset(ActionMapping mapping, HttpServletRequest request) {
//        this.query = null;
//        this.del = null;
//    }
}
