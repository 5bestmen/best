package com.corsair.entity;

/**
 * Created by 洪祥 on 15/7/27.
 */
public class OperateTicketTemplateData implements java.io.Serializable {
    String id;
    int indexNumber;

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    String content;
    String comments;
    BaseTicket baseTicket;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }


    public int getIndexNumber() {
        return indexNumber;
    }

    public void setIndexNumber(int indexNumber) {
        this.indexNumber = indexNumber;
    }

    public String getComments() {
        return comments;
    }

    public void setComments(String comments) {
        this.comments = comments;
    }


    public BaseTicket getBaseTicket() {
        return baseTicket;
    }

    public void setBaseTicket(BaseTicket baseTicket) {
        this.baseTicket = baseTicket;
    }
}
