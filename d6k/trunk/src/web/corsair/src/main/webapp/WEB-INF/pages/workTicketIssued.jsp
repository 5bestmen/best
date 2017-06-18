<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<%@ include file="/common/taglibs.jsp"%>
<!DOCTYPE html>
<html lang="zh_CN">

<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>签发</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">


    <link href="<c:url value="/resources/dist/js/html5shiv.min.js"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/dist/js/respond.min.js"/>" rel="stylesheet" type="text/css">

    <script src="<c:url value="/resources/dist/laydate/laydate.js"/>"></script>
    <%--todo 页面自身需要的CSS begin--%>
    <link href="<c:url value="/resources/bower_components/dtGrid/css/jquery.dtGrid.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/bower_components/datePicker/skin/WdatePicker.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/bower_components/datePicker/skin/default/datepicker.css"/>" rel="stylesheet" type="text/css">
    <%--todo 页面自身需要的CSS end--%>
</head>

<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/bower_components/dtGrid/js/jquery.dtGrid.min.js"/>"></script>
<script src="<c:url value="/resources/bower_components/dtGrid/i18n/zh-cn.js"/>"></script>
<script src="<c:url value="/resources/bower_components/datePicker/WdatePicker.js"/>"></script>
<script src="<c:url value="/resources/js/workTicket/validate.js"/>"></script>
<style type="text/css">
    .note{color: red}
</style>
<%--<div id="wrapper">
    <jsp:include page="meun.jsp"/>

    &lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
    <div id="page-wrapper">--%>
        <div class="row">
            <div class="col-lg-22">
                <h2 class="page-header">签发工作票</h2>
            </div>
            <!-- /.col-lg-22 -->
        </div>
        <form:form cssClass="form-horizontal" action="workTicketIssued" commandName="workTicketModifyForm" onsubmit="return toValid()">
            <form:hidden path="id"/>
            <form:hidden path="processId"/>
            <form:hidden path="orderId"/>
            <form:hidden path="taskId"/>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">工作票类型</label>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:select cssClass="form-control"  path="workTicketType" items="${workTicketTypes}" disabled="true"></form:select>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">工作责任人</label>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:input type="text" cssClass="form-control" path="responsiblePerson" disabled="true"/>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">工作班成员</label>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:input cssClass="form-control" type="text"  path="crew" disabled="true"></form:input>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">部门和班组</label>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:input type="text" cssClass="form-control" path="department" disabled="true"/>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">计划工作开始时间</label>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:input cssClass="form-control laydate-icon layer-date" placeholder="计划工作时间" path="plannedWorkingStartTime"  disabled="true"/>
                </div>
            </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">计划工作结束时间</label>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:input cssClass="form-control laydate-icon layer-date" placeholder="计划工作时间" path="plannedWorkingEndTime"  disabled="true"/>
                </div>
            </div>
            &nbsp;&nbsp;
            <div id = "taskMeasure">
            <c:if test="${not empty workContents}">
                <c:forEach items="${workContents}" var="workContents">

                    <div class="form-group items">
                        <div class="col-sm-2 col-lg-2">
                            <label class="control-label">工作地点及设备双重名称</label>
                        </div>
                        <div class="col-lg-4 col-xs-8">
                            <textarea  class="form-control"name="workNameAndPlace" rows="2" cols="300" type="text" disabled="true" placeholder="工作地点及设备双重名称">${workContents.taskName}</textarea>
                        </div>
                        <div class="col-sm-2 col-lg-1">
                            <label class="control-label">工作内容</label>
                        </div>
                        <div class="col-lg-4 col-xs-8">
                            <textarea  class="form-control"name="workContent" rows="2" cols="300" type="text"  disabled="true" placeholder="工作内容">${workContents.taskContent}</textarea>
                        </div>
                    </div>

                </c:forEach>
            </c:if>
        </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">工作条件</label>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:textarea  cssClass="form-control" path="workCondition" disabled="true"/>
                </div>
            </div>
            <c:if test="${'4' eq workTicketType  }">
            <div class="form-group" id="secondRemark">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">备注</label>
                </div>
                <div class="col-xs-8 col-lg-4">
                    <form:textarea cssClass="form-control" placeholder="备注" path="remark" disabled="true"/>
                </div>
            </div>
            </c:if>
            <c:if test="${'3' eq  workTicketType }">
                <div class="form-group" id="oneRemark">
                    &nbsp;&nbsp;<h4>备注</h4>
                    <div class="col-sm-2 col-lg-2">
                        <label class="control-label">指定专职监护人</label>
                    </div>
                    <div class="col-xs-8 col-lg-3">
                        <form:select cssClass="form-control"  path="fullTimePerson" items="${accoutRealNames}" disabled="true"></form:select>
                    </div>
                    <div class="col-sm-2 col-lg-2">
                        <label class="control-label">负责监护</label>
                    </div>
                    <div class="col-xs-8 col-lg-4">
                        <form:textarea cssClass="form-control" placeholder="地点及具体工作" path="fullRemark" disabled="true"/>
                    </div>
                    <div class="col-sm-2 col-lg-2">
                        <label class="control-label">其他事项</label>
                    </div>
                    <div class="col-xs-8 col-lg-4">
                        <form:textarea cssClass="form-control" placeholder="其他事项" path="otherRemark" disabled="true"/>
                    </div>
                </div>
            </c:if>

            <div class="form-group">
                &nbsp;&nbsp;<h4>工作票流程审核人设置：</h4>
            </div>
            <div class="form-group">
            <div class="col-sm-2 col-lg-2">
                <label class="control-label">流程许可人</label>
                <span style="color:red" title="必填">(*)</span>
            </div>
            <div class="col-xs-8 col-lg-3">
                <form:select cssClass="form-control"  path="processPermitPerson" items="${accoutRealNames}"></form:select>
            </div>
                <span class="note" id="permitNote">(*)必填</span>
        </div>
            <div>
                <c:if test="${not empty securityMeasureses}">
                    <c:forEach items="${securityMeasureses}" var="securityMeasures">
                        <div class="form-group items">
                            <div class="col-sm-2 col-lg-2">
                                <label class="control-label">安全措施类型</label>
                            </div>
                            <c:if test="${not empty securityMeasures.securityMeasuresType}">
                            <div class="col-sm-4 col-xs-8">
                                <textarea  class="form-control"name="securityMeasureType" rows="2" cols="300" type="text" placeholder="安全措施类型" disabled="true">${securityMeasures.securityMeasuresType}</textarea>
                            </div>
                            </c:if>
                            <div class="col-lg-5 col-xs-8">
                                <textarea  class="form-control"name="securityMeasure" rows="2" cols="300" type="text" placeholder="安全措施" disabled="true">${securityMeasures.measure}</textarea>
                            </div>
                        </div>

                    </c:forEach>
                </c:if>
            </div>
            <div class="form-group">
                <div class="col-xs-6">
                    <button type="button" class="btn btn-primary" id="addNeedAttention">注意事项</button>
                </div>
            </div>
            &nbsp;&nbsp;
            <div id="needAttention">
                <c:if test="${not empty needAttentions}">
                    <c:forEach items="${needAttentions}" var="needAttentions">

                        <div class="form-group items">
                            <div class="col-sm-2 col-lg-2">
                                <label class="control-label">工作地点保留带电部分或注意事项</label>
                            </div>
                            <div class="col-lg-5 col-xs-8">
                                <textarea  class="form-control" name="attention" rows="2" cols="300" type="text" placeholder="工作地点保留带电部分或注意事项">${needAttentions.needAttention}</textarea>
                            </div>
                        </div>

                    </c:forEach>
                </c:if>
            </div>
            <form:input type="submit" cssClass="btn"  path="issued" value="签发"/>
            <form:input type="submit" cssClass="btn" path="quit" value="放弃"/>
        </form:form>
    <%--</div>
</div>--%>
        <script>

            $(function() {
                //添加安全措施——模板2
                var max_item_num = 15;
                var item_num = $('#ticket .items').length;
                if (item_num > 0){
                    $('.del-item').click(function (){
                        $(this).parent().parent().remove();
                        item_num--;
                    });
                }

                $('#addNeedAttention').click(function () {

                    if (item_num < max_item_num) {
                        $('#needAttention').append('<div class="form-group items">'+
                        '<div class="col-sm-2 col-lg-2">'+
                        '<label class="control-label">注意事项</label>'+
                        '</div>'+
                        '<div class="col-lg-5 col-xs-8">'+
                        '<textarea  class="form-control" name="attention" rows="2" cols="300" type="text" placeholder="工作地点保留带电部分或注意事项"></textarea>'+
                        '</div>'+
                        '<div class="col-lg-1">'+
                        '<button type="button"  class="del-item btn btn-danger">删除</button>'+
                        '</div>'+
                        '<div class="col-lg-1">'+
                        '</div>');
                        item_num++;

                    } else {
                        alert("工作地点保留带电部分或注意事项过多，请扼要描述！");
                    }
                    $('.del-item').unbind();
                    $('.del-item').on('click', function () {
                        $(this).parent().parent().remove();
                        item_num--;
                    });
                });
            });

            function toValid(){
                //校验许可人
                if(!checkNull("#processPermitPerson","#permitNote")){
                    return false;
                }
                //校验注意事项唯一性
                if(isRepeat("textarea[name=attention]")){
                    return false;
                }
                return true;
            }
        </script>
</div>

</body>

</html>