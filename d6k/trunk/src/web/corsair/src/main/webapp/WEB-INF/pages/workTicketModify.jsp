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

    <title>填写工作票</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">


    <link href="<c:url value="/resources/dist/js/html5shiv.min.js"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/dist/js/respond.min.js"/>" rel="stylesheet" type="text/css">

    <script src="<c:url value="/resources/dist/laydate/laydate.js"/>"></script>
    <%--todo 页面自身需要的CSS begin--%>
    <link href="<c:url value="/resources/bower_components/dtGrid/css/jquery.dtGrid.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/bower_components/datePicker/skin/WdatePicker.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/bower_components/datePicker/skin/default/datepicker.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

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
            <div class="col-lg-12">
                <h3 class="page-header">填写工作票</h3>
            </div>
            <!-- /.col-lg-22 -->
        </div>
        <form:form cssClass="form-horizontal" action="workTicketModify" commandName="workTicketModifyForm" onsubmit="return toValid()">
            <form:hidden path="id"/>
            <form:hidden path="processId"/>
            <form:hidden path="orderId"/>
            <form:hidden path="taskId"/>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">工作票类型</label>
                </div>
                <div class="col-xs-4 col-lg-3">
                    <form:select cssClass="form-control"  path="workTicketType" items="${workTicketTypes}" onchange="selectWorkTicketType()"></form:select>
                </div>
            </div>
            <%--<div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">安全措施类型</label>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:select cssClass="form-control"  path="securityMeasuresType" items="${securityMeasuresTypes}"></form:select>
                </div>
            </div>--%>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">工作班成员</label>
                </div>
                <div class="col-xs-4 col-lg-3">
                    <form:select cssClass="form-control"  path="crew" items="${accoutRealNames}" multiple="multiple"></form:select>
                </div>
                <span class="note" id="crewNote">按住ctrl键可以多选，必填</span>
            </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">部门和班组</label>
                </div>
                <div class="col-xs-4 col-lg-3">
                    <form:input type="text" cssClass="form-control" path="department" onblur="checkNull('#department','#noteDepart')"/>
                </div>
                <span class="note" id="noteDepart">(*)必填</span>
            </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">计划工作开始时间</label>
                </div>
                <div class="col-xs-4 col-lg-3">
                    <form:input cssClass="form-control laydate-icon layer-date" placeholder="计划工作开始时间" path="plannedWorkingStartTime"/>
                </div>
                <span class="note" id="startNote">(*)必填</span>
            </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">计划工作结束时间</label>
                </div>
                <div class="col-xs-4 col-lg-3">
                    <form:input cssClass="form-control laydate-icon layer-date" placeholder="计划工作结束时间" path="plannedWorkingEndTime"/>
                </div>
                <span class="note" id="endNote">(*)必填</span>
            </div>
            <div class="form-group" id="condition" style="display: none">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">工作条件</label>
                </div>
                <div class="col-xs-4 col-lg-3">
                    <form:textarea  cssClass="form-control" path="workCondition"/>
                </div>
            </div>
            <div class="form-group" id="secondRemark" style="display: none">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">备注</label>
                </div>
                <div class="col-xs-4 col-lg-4">
                    <form:textarea cssClass="form-control" placeholder="备注" path="remark"/>
                </div>
            </div>
            <div class="form-group" id="oneRemark">
                &nbsp;&nbsp;<h4>备注</h4>
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">指定专职监护人</label>
                </div>
                <div class="col-xs-4 col-lg-3">
                    <form:select cssClass="form-control"  path="fullTimePerson" items="${accoutRealNames}"></form:select>
                </div>
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">负责监护</label>
                </div>
                <div class="col-xs-4 col-lg-4">
                    <form:textarea cssClass="form-control" placeholder="地点及具体工作" path="fullRemark"/>
                </div>
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">其他事项</label>
                </div>
                <div class="col-xs-4 col-lg-4">
                    <form:textarea cssClass="form-control" placeholder="其他事项" path="otherRemark"/>
                </div>
            </div>
            <div class="form-group">
                &nbsp;&nbsp;<h4>工作票流程审核人设置：</h4>
            </div>
                <div class="form-group">
                    <div class="col-sm-2 col-lg-2">
                        <label class="control-label">流程签发人</label>
                        <span style="color:red" title="必填">(*)</span>
                    </div>
                    <div class="col-xs-4 col-lg-3">
                        <form:select cssClass="form-control"  path="processIssuedPerson" items="${accoutRealNames}"></form:select>
                    </div>
                    <span class="note" id="issueNote"></span>
                </div>
            &nbsp;&nbsp;
            <div class="form-group">
                <div class="col-xs-4">
                    <button type="button" id="addTaskItem" class="btn btn-primary">增加工作任务</button>
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
                            <div class="col-lg-4 col-xs-3">
                                <textarea  class="form-control"name="workNameAndPlace" rows="2" cols="300" type="text" placeholder="工作地点及设备双重名称">${workContents.taskName}</textarea>
                            </div>
                            <div class="col-sm-2 col-lg-1">
                                <label class="control-label">工作内容</label>
                            </div>
                            <div class="col-lg-4 col-xs-3">
                                <textarea  class="form-control"name="workContent" rows="2" cols="300" type="text" placeholder="工作内容">${workContents.taskContent}</textarea>
                            </div>
                            <div class="col-lg-1 col-xs-1">
                                <button type="button"  class="del-item btn btn-danger">删除</button>
                            </div>
                        </div>

                    </c:forEach>
                </c:if>
            </div>
            <div class="form-group">
                <div class="col-xs-4">
                    <button type="button" id="addItem" class="btn btn-primary">增加安全措施</button>
                </div>
            </div>
            &nbsp;&nbsp;
            <div id = "measure">
                <c:if test="${not empty securityMeasureses}">
                    <c:forEach items="${securityMeasureses}" var="securityMeasures">

                        <div class="form-group items">
                            <div class="col-sm-2 col-lg-2">
                                <label class="control-label">安全措施</label>
                            </div>
                            <div class="col-sm-2 col-lg-2">
                                <select name="securityMeasuresType">
                                    <option value=""></option>
                                    <option value="1">1</option>
                                    <option value="2">2</option>
                                    <option value="3">3</option>
                                </select>
                            </div>
                            <div class="col-lg-5 col-xs-4">
                                <textarea  class="form-control"name="securityMeasure" rows="2" cols="300" type="text" placeholder="安全措施">${securityMeasures.measure}</textarea>
                            </div>
                            <div class="col-lg-1 col-xs-1">
                                <button type="button"  class="del-item btn btn-danger">删除</button>
                            </div>
                        </div>

                    </c:forEach>
                </c:if>
            </div>
            <form:input type="submit" cssClass="btn"  path="submit" value="保存"/>
            <input class="btn" type="button" value="返回" onclick="history.back();" />
        </form:form>
        <script>

            $(function() {
                var data = $("#fullTimePerson").find("option:selected").val();
                if(data=="---请选择---"){
                    $("#fullTimePerson").find("option:selected").remove();
                }

                var obj_v=$("#workTicketType").val();
                if("电气一种工作票"==obj_v){
                    $("#oneRemark").show();
                    $("#secondRemark").hide();
                }else if("电气二种工作票"==obj_v){
                    $("#oneRemark").hide();
                    $("#secondRemark").show();
                }
               //添加安全措施——模板2
                $("#oneRemark").show();
                var max_item_num = 15;
                var item_num = $('#ticket .items').length;
                if (item_num > 0){
                    $('.del-item').click(function (){
                        $(this).parent().parent().remove();
                        item_num--;
                    });
                }
                $('#workTicketType').change(function (){
                    var type = $("#workTicketType").val();
                    if(type == "电气二种工作票"){
                        $("div[name=add]").remove();
                        $("#condition").show();
                    }else if(type == "电气一种工作票")
                    {
                        $("div[name=content]").remove();
                        $("div[name=safe]").remove();
                        $("div[name=del]").remove();
                        $("div[name=add]").show();
                        $("#condition").hide();

                    }

                });
                $('#addItem').click(function () {
                    var str='';
                    if (item_num < max_item_num) {
                        str ='<div class="form-group items" name="safe">';
                        str += '<div class="col-sm-2 col-lg-2">';
                        str +='<label class="control-label">安全措施</label>';
                        str += '</div>';

                        var type = $("#workTicketType").val();
                        if(type == "电气一种工作票"){
                            str += '<div class="col-sm-2 col-lg-3" name="add">';
                            str +='<select name="securityMeasuresType" class="form-control">';
                            str +='<option value="1">应拉断路器（开关）、隔离开关(刀闸)</option>';
                            str +='<option value="2">应装接地线、应合接地刀闸（注明确实地点、名称及接地线编号）</option>';
                            str +='<option value="3">应设遮栏、应挂标示牌及防止二次回路误碰等措施</option>';
                            str +='</select>';
                            str +='</div>';
                        }
                        str += '<div class="col-lg-5 col-xs-3" name="content">';
                        str += '<textarea  class="form-control" name="securityMeasure" rows="2" cols="300" type="text" placeholder="安全措施"></textarea>';
                        str +='</div>';
                        str +='<div class="col-lg-1" name="del">';
                        str += '<button type="button"  class="del-item btn btn-danger">删除</button>';
                        str += '</div>';
                        str += '<div class="col-lg-1">';
                        str += '</div>';
                        $("#measure").append(str);
                        item_num++;
                    } else {
                        alert("安全措施过多，请扼要描述！");
                    }
                    $('.del-item').unbind();
                    $('.del-item').on('click', function () {
                        $(this).parent().parent().remove();
                        item_num--;
                    });
                });

                //添加工作内容

                $('#addTaskItem').click(function () {

                    if (item_num < max_item_num) {
                        $('#taskMeasure').append('<div class="form-group items">'+
                        '<div class="col-sm-2 col-lg-2">'+
                        '<label class="control-label">工作地点及设备双重名称</label>'+
                        '</div>'+
                        '<div class="col-lg-4 col-xs-3">'+
                        '<textarea  class="form-control" name="workNameAndPlace" rows="2" cols="300" type="text" placeholder="工作地点及设备双重名称"></textarea>'+
                        '</div>'+
                        '<div class="col-sm-2 col-lg-1">'+
                        '<label class="control-label">工作内容</label>'+
                        '</div>'+
                        '<div class="col-lg-4 col-xs-3">'+
                        '<textarea  class="form-control" name="workContent" rows="2" cols="300" type="text" placeholder="工作内容"></textarea>'+
                        '</div>'+
                        '<div class="col-lg-1">'+
                        '<button type="button"  class="del-item btn btn-danger">删除</button>'+
                        '</div>'+
                        '</div>');
                        item_num++;

                    } else {
                        alert("工作任务过多，请扼要描述！");
                    }
                    $('.del-item').unbind();
                    $('.del-item').on('click', function () {
                        $(this).parent().parent().remove();
                        item_num--;
                    });

                });
            });

            function selectWorkTicketType(){
                var obj_v=$("#workTicketType").val();
                if("电气一种工作票"==obj_v){
                    $("#oneRemark").show();
                    $("#secondRemark").hide();
                }else if("电气二种工作票"==obj_v){
                    $("#oneRemark").hide();
                    $("#secondRemark").show();
                }
            }

            function toValid(){
                var data = $("select[name=securityMeasuresType]").find("option:selected");
                var workContent = $("textarea[name=workNameAndPlace]");

                //校验工作班成员
                if(!checkNull("#crew","#crewNote")){
                    return false;
                }
                //校验部门和班组
                if(!checkNull("#department","#noteDepart")){
                    return false;
                }
                //校验工作任务唯一性
                if(notRepeat(workContent,"textarea[name=workContent]")){
                    return false;
                }
                //校验安全措施唯一性
                if(notRepeat(data,"textarea[name=securityMeasure]")){
                    return false;
                }

                //校验计划开始时间，非空
                if(!checkNull("#plannedWorkingStartTime","#startNote")){
                    return false;
                }
                //校验计划结束时间，非空
                if(!checkNull("#plannedWorkingEndTime","#endNote")){
                    return false;
                }
                //校验计划结束时间，必须大于结束时间
                if(!compareTime("#plannedWorkingStartTime","#plannedWorkingEndTime")){
                    return false;
                }
                //校验签发人
                if(!checkNull("#processIssuedPerson","#issueNote")){
                    return false;
                }
                return true;
            }

            var timeStart = {
                elem: '#plannedWorkingStartTime',
                format: 'YYYY-MM-DD hh:mm:ss',
                max: '2099-06-16',
                istime: true,
                istoday: true
            }
            var timeEnd = {
                elem: '#plannedWorkingEndTime',
                format: 'YYYY-MM-DD hh:mm:ss',
                max: '2099-06-16',
                istime: true,
                istoday: true
            }
            laydate(timeStart);
            laydate(timeEnd);
        </script>
    <%--</div>

</div>--%>

</body>

</html>