package com.corsair.form;

/**
 * Created by zjq on 2015/11/5.
 */
public class MaterialInformationForm{

    String currentNo;
    String totalPage;
    String currentPage;
    String displayNum;
    String search;
    String prev;
    String next;
    String materialId;
    String materialSeriesNo;

    public String getCurrentNo() {
        return currentNo;
    }

    public void setCurrentNo(String currentNo) {
        this.currentNo = currentNo;
    }

    public String getTotalPage() {
        return totalPage;
    }

    public void setTotalPage(String totalPage) {
        this.totalPage = totalPage;
    }

    public String getCurrentPage() {
        return currentPage;
    }

    public void setCurrentPage(String currentPage) {
        this.currentPage = currentPage;
    }

    public String getDisplayNum() {
        return displayNum;
    }

    public void setDisplayNum(String displayNum) {
        this.displayNum = displayNum;
    }

    public String getPrev() {
        return prev;
    }

    public void setPrev(String prev) {
        this.prev = prev;
    }

    public String getNext() {
        return next;
    }

    public void setNext(String next) {
        this.next = next;
    }

    public String getSearch() {
        return search;
    }

    public void setSearch(String search) {
        this.search = search;
    }

    public String getMaterialId() {
        return materialId;
    }

    public void setMaterialId(String materialId) {
        this.materialId = materialId;
    }

    public String getMaterialSeriesNo() {
        return materialSeriesNo;
    }

    public void setMaterialSeriesNo(String materialSeriesNo) {
        this.materialSeriesNo = materialSeriesNo;
    }
}
