package com.rbac.common;

import com.corsair.device.AlarmRecordData;

import java.io.Serializable;
import java.util.List;

/**
 * Created by zyj on 2017/3/16.
 */
public class PagerOfAlarm {

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }

    public int getCurrentPage() {
        return currentPage;
    }

    public void setCurrentPage(int currentPage) {
        this.currentPage = currentPage;
    }

    public int getTotalRecord() {
        return totalRecord;
    }

    public void setTotalRecord(int totalRecord) {
        this.totalRecord = totalRecord;
    }

    public int getTotalPage() {
        return totalPage;
    }

    public void setTotalPage(int totalPage) {
        this.totalPage = totalPage;
    }

    public List<AlarmRecordData> getDataList() {
        return dataList;
    }

    public void setDataList(List<AlarmRecordData> dataList) {
        this.dataList = dataList;
    }

    /**
     * 序列化id
     */
    private int pageSize;//每页显示多少条记录
    private int currentPage;//当前第几页数据
    private int totalRecord;//一共多少条记录
    private int totalPage;//总页数
    private List<AlarmRecordData> dataList;//要显示的数据

    public PagerOfAlarm(int pageNum,int pageSize,List sourceList) {

        if(sourceList.size() ==0 || sourceList == null) {
            return;
        }
        //总记录条数
        this.totalRecord = sourceList.size();
        //每页显示多少条记录
        this.pageSize = pageSize;
        //获取总页数
        this.totalPage = this.totalRecord /this.pageSize;
        if(this.totalRecord % this.pageSize != 0){
            this.totalPage = this.totalPage +1;
        }
        //当前第几页数据
        if(this.totalPage < pageNum){
            this.currentPage = this.totalPage;
        }else{
            this.currentPage = pageNum;
        }

        //起始索引
        int fromIndex = this.pageSize*(this.currentPage-1);
        //结束索引
        int toIndex;
        if(this.pageSize * this.currentPage >this.totalRecord){
            toIndex = this.totalRecord;
        }else{
            toIndex = this.pageSize * this.currentPage;
        }
        this.dataList = sourceList.subList(fromIndex, toIndex);
    }
}
