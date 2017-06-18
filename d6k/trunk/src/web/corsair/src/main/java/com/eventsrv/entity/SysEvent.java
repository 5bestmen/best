package com.eventsrv.entity;

import java.util.UUID;

/**
 * Created by ChengXi on 2015/12/28.
 */
public class SysEvent implements java.io.Serializable {

    private String id;
    private short  read;               // 是否已读
    private short grpno;              //0、分号组
    private short sort;					//1、事件分类号
    private short type;					//2、事件类型
    private int ymd;						//3、年月日
    private int hmsms;					//4、时分秒毫秒
    private short security;					//5、安全级
    private short volgrade;					//6	电压等级
    private short testflag;				//7	实验标志
    private short PrintOut;
    private short DiskOut;
    private short ToneOut;
    private short GraphOut;
    private short DispOut;
    private short state;
    private short state1;
    private short state2;
    private float val;						//9.当前浮点值
    private float val1;
    private float val2;
    private float val3;
    private String Object;		//13.事件对象组名
    private String member0;	//14.事件对象名0
    private String member1;	//15.事件对象名1
    private String member2;	//16.事件对象名2
    private String member3;	//17.事件对象名3
    private String graph;			//18、推图名
    private String tone;	//19、语音描述信息
    private String text;	//20、文字描述信息

    public SysEvent() {
        this.id = UUID.randomUUID().toString();
        this.read = 0;

    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public short getRead() {
        return read;
    }

    public void setRead(short read) {
        this.read = read;
    }

    public short getGrpno() {
        return grpno;
    }

    public void setGrpno(short grpno) {
        this.grpno = grpno;
    }

    public short getSort() {
        return sort;
    }

    public void setSort(short sort) {
        this.sort = sort;
    }

    public short getType() {
        return type;
    }

    public void setType(short type) {
        this.type = type;
    }

    public int getYmd() {
        return ymd;
    }

    public void setYmd(int ymd) {
        this.ymd = ymd;
    }

    public int getHmsms() {
        return hmsms;
    }

    public void setHmsms(int hmsms) {
        this.hmsms = hmsms;
    }

    public short getSecurity() {
        return security;
    }

    public void setSecurity(short security) {
        this.security = security;
    }

    public short getVolgrade() {
        return volgrade;
    }

    public void setVolgrade(short volgrade) {
        this.volgrade = volgrade;
    }

    public short getTestflag() {
        return testflag;
    }

    public void setTestflag(short testflag) {
        this.testflag = testflag;
    }

    public short getPrintOut() {
        return PrintOut;
    }

    public void setPrintOut(short printOut) {
        PrintOut = printOut;
    }

    public short getDiskOut() {
        return DiskOut;
    }

    public void setDiskOut(short diskOut) {
        DiskOut = diskOut;
    }

    public short getToneOut() {
        return ToneOut;
    }

    public void setToneOut(short toneOut) {
        ToneOut = toneOut;
    }

    public short getGraphOut() {
        return GraphOut;
    }

    public void setGraphOut(short graphOut) {
        GraphOut = graphOut;
    }

    public short getDispOut() {
        return DispOut;
    }

    public void setDispOut(short dispOut) {
        DispOut = dispOut;
    }

    public short getState() {
        return state;
    }

    public void setState(short state) {
        this.state = state;
    }

    public short getState1() {
        return state1;
    }

    public void setState1(short state1) {
        this.state1 = state1;
    }

    public short getState2() {
        return state2;
    }

    public void setState2(short state2) {
        this.state2 = state2;
    }

    public float getVal() {
        return val;
    }

    public void setVal(float val) {
        this.val = val;
    }

    public float getVal1() {
        return val1;
    }

    public void setVal1(float val1) {
        this.val1 = val1;
    }

    public float getVal2() {
        return val2;
    }

    public void setVal2(float val2) {
        this.val2 = val2;
    }

    public float getVal3() {
        return val3;
    }

    public void setVal3(float val3) {
        this.val3 = val3;
    }

    public String getObject() {
        return Object;
    }

    public void setObject(String object) {
        Object = object;
    }

    public String getMember0() {
        return member0;
    }

    public void setMember0(String member0) {
        this.member0 = member0;
    }

    public String getMember1() {
        return member1;
    }

    public void setMember1(String member1) {
        this.member1 = member1;
    }

    public String getMember2() {
        return member2;
    }

    public void setMember2(String member2) {
        this.member2 = member2;
    }

    public String getMember3() {
        return member3;
    }

    public void setMember3(String member3) {
        this.member3 = member3;
    }

    public String getGraph() {
        return graph;
    }

    public void setGraph(String graph) {
        this.graph = graph;
    }

    public String getTone() {
        return tone;
    }

    public void setTone(String tone) {
        this.tone = tone;
    }

    public String getText() {
        return text;
    }

    public void setText(String text) {
        this.text = text;
    }
}
