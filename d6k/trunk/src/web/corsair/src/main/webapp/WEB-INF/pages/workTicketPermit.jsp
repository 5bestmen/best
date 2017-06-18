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

    <title>工作票审结</title>

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
                <h2 class="page-header">工作票审结</h2>
            </div>
            <!-- /.col-lg-22 -->
        </div>
        <form:form cssClass="form-horizontal" action="workTicketPermit" commandName="workTicketModifyForm" onsubmit="return toValid()">
            <form:hidden path="id"/>
            <form:hidden path="processId"/>
            <form:hidden path="orderId"/>
            <form:hidden path="taskId"/>
            <form:hidden  path="selectResponsiblePerson" id="changeFuZeRen"/>
            <form:hidden  path="selectdelayTime" id="changeDelayDate"/>
            <form:hidden  path="responseResetPerson" id="resetResponsePerson"/>
            <form:hidden  path="changeStartTime" id="planStartTime"/>
            <form:hidden  path="changeEndTime" id="planEndTime"/>
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
                <div class="col-xs-8 col-lg-3 form-inline">
                    <form:select cssClass="form-control"  path="responsiblePerson" items="${accoutRealNames}" disabled="true" id="workPerson"></form:select>&nbsp;
                    <span id="change"><input type="checkbox"   id="changePerson" name="checkbox1" onclick="changePersonCheck()" value=""/>是否变更负责人</span>
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
                    <form:input cssClass="form-control laydate-icon layer-date" placeholder="计划工作开始时间" path="plannedWorkingStartTime"  disabled="true" />&nbsp;<input type="checkbox"  name="checkbox1" id="changeDelay" onclick="changeDelayCheck()"/>是否延期
                </div>
                <span class="note" id="startNote">(*)必填</span>
            </div>
            <div class="form-group">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">计划工作结束时间</label>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:input cssClass="form-control laydate-icon layer-date" placeholder="计划工作结束时间" path="plannedWorkingEndTime"  disabled="true"/>
                </div>
                <span class="note" id="endNote">(*)必填</span>
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
                            <div class="col-lg-1 col-xs-1">
                                <button type="button"  class="del-item btn btn-danger" disabled="true">删除</button>
                            </div>
                        </div>

                    </c:forEach>
                </c:if>
            </div>
            <div id="needAttention">
                <c:if test="${not empty needAttentions}">
                    <c:forEach items="${needAttentions}" var="needAttentions">

                        <div class="form-group items">
                            <div class="col-sm-2 col-lg-2">
                                <label class="control-label">工作地点保留带电部分或注意事项</label>
                            </div>
                            <div class="col-lg-5 col-xs-8">
                                <textarea  class="form-control" name="attention" rows="2" cols="300" type="text" disabled="true" placeholder="工作地点保留带电部分或注意事项">${needAttentions.needAttention}</textarea>
                            </div>
                        </div>

                    </c:forEach>
                </c:if>
            </div>
            <c:if test="${'4' eq  workTicketType }">
            <div class="form-group">
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
            <div class="form-group" id="setEndPerson">
                &nbsp;&nbsp;<h4>工作票流程审核人设置：</h4>
            </div>
            <div class="form-group" id="setEndPersonContent">
                <div class="col-sm-2 col-lg-2">
                    <label class="control-label">工作终结人</label>
                    <span style="color:red" title="必填">(*)</span>
                </div>
                <div class="col-xs-8 col-lg-3">
                    <form:select cssClass="form-control"  path="permitPerson" items="${accoutRealNames}"></form:select>
                </div>
                <span class="note" id="permitNote">(*)必填</span>
            </div>

            <div class="form-group">
                <div class="col-xs-6">
                    <button type="button" id="addPermit" class="btn btn-primary">增加工作审结</button>
                </div>
            </div>
            <div id = "permit">
                <c:if test="${not empty workPermitses}">
                    <c:forEach items="${workPermitses}" var="workPermits">

                        <div class="form-group items">
                            <div class="col-sm-2 col-lg-2">
                                <label class="control-label">工作审结</label>
                            </div>
                            <div class="col-lg-5 col-xs-8">
                                <textarea  class="form-control"name="workPermit" rows="2" cols="300" type="text" placeholder="工作审结">${workPermits.permits}</textarea>
                            </div>
                            <div class="col-lg-1 col-xs-1">
                                <button type="button"  class="del-item btn btn-danger">删除</button>
                            </div>
                        </div>

                    </c:forEach>
                </c:if>
            </div>
            <div class="form-group">
                <div class="col-xs-6">
                    <button type="button" class="btn btn-primary" id="addNeedAttentionMeasureses">补充工作地点保留带电部分和安全措施</button>
                </div>
            </div>
            &nbsp;&nbsp;
            <div id="attentions">
                <c:if test="${not empty attenTionMeasure}">
                    <c:forEach items="${attenTionMeasure}" var="attenTionMeasure">

                        <div class="form-group items">
                            <div class="col-sm-2 col-lg-2">
                                <label class="control-label">补充工作地点保留带电部分和安全措施</label>
                            </div>
                            <div class="col-lg-5 col-xs-8">
                                <textarea  class="form-control" name="attentionMeasureses" rows="2" cols="300" type="text" placeholder="补充工作地点保留带电部分和安全措施">${attenTionMeasure.attenTionMeasure}</textarea>
                            </div>
                        </div>

                    </c:forEach>
                </c:if>
            </div>
            <form:input type="submit" cssClass="btn"  path="agree" value="通过"/>
            <form:input type="submit" cssClass="btn"  path="disagree" value="不通过"/>
        </form:form>
        <script>

            $(function() {
                $("#resetResponsePerson").val($("#workPerson").val());
                if(true==${isChangePerson}){
                    $("#changeFuZeRen").val("1");
                    $("#changePerson").attr("checked",true);
                    $("#workPerson").attr("disabled",false);
                    $("#setEndPerson").hide();
                    $("#setEndPersonContent").hide();
                }else{
                    $("#changeFuZeRen").val("0");
                    $("#changePerson").attr("checked",false);
                    $("#workPerson").attr("disabled",true);
                    $("#setEndPerson").show();
                    $("#setEndPersonContent").show();
                }
                if(true==${isChangeDelay}){
                    $("#changeDelay").attr("checked",true);
                    $("#plannedWorkingStartTime").attr("disabled",false);
                    $("#plannedWorkingEndTime").attr("disabled",false);
                    $("#changeDelayDate").val("1");
                    $("#setEndPerson").hide();
                    $("#setEndPersonContent").hide();
                }else{
                    $("#changeDelay").attr("checked",false);
                    $("#plannedWorkingStartTime").attr("disabled",true);
                    $("#plannedWorkingEndTime").attr("disabled",true);
                    $("#changeDelayDate").val("0");
                    $("#setEndPerson").show();
                    $("#setEndPersonContent").show();
                }
                var max_item_num = 10;
                var item_num = $('#ticket .items').length;
                if (item_num > 0){
                    $('.del-item').click(function (){
                        $(this).parent().parent().remove();
                        item_num--;
                    });
                }

                $('#addPermit').click(function () {

                    if (item_num < max_item_num) {
                        $('#permit').append('<div class="form-group items">'+
                        '<div class="col-sm-2 col-lg-2">'+
                        '<label class="control-label">工作审结</label>'+
                        '</div>'+
                        '<div class="col-lg-5 col-xs-8">'+
                        '<textarea  class="form-control" name="workPermit" rows="2" cols="300" type="text" placeholder="工作审结"></textarea>'+
                        '</div>'+
                        '<div class="col-lg-1">'+
                        '<button type="button"  class="del-item btn btn-danger">删除</button>'+
                        '</div>'+
                        '<div class="col-lg-1">'+
                        '</div>');
                        item_num++;

                    } else {
                        alert("工作审结过多，请扼要描述！");
                    }
                    $('.del-item').unbind();
                    $('.del-item').on('click', function () {
                        $(this).parent().parent().remove();
                        item_num--;
                    });
                });
                    $('#addNeedAttentionMeasureses').click(function () {
                        if (item_num < max_item_num) {
                            $('#attentions').append('<div class="form-group items">'+
                            '<div class="col-sm-2 col-lg-2">'+
                            '<label class="control-label">补充工作地点保留带电部分和安全措施</label>'+
                            '</div>'+
                            '<div class="col-lg-5 col-xs-8">'+
                            '<textarea  class="form-control" name="attentionMeasureses" rows="2" cols="300" type="text" placeholder="补充工作地点保留带电部分和安全措施"></textarea>'+
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
            function changePersonCheck(){
                if($("#changePerson").is(":checked")){
                    $("#workPerson").attr("disabled",false);
                    $("#changeFuZeRen").val("1");
                    $("#setEndPerson").hide();
                    $("#setEndPersonContent").hide();
                    checkCheckNum();
                }else{
                    $("#resetResponsePerson").val($("#workPerson").val());
                    $("#planStartTime").val($("#plannedWorkingStartTime").val());
                    $("#planEndTime").val($("#plannedWorkingEndTime").val());
                    $("#workPerson").attr("disabled",true);
                    $("#changeFuZeRen").val("0");
                    $("#setEndPerson").show();
                    $("#setEndPersonContent").show();
                }
            }
            function changeDelayCheck(){
                if($("#changeDelay").is(":checked")){
                    $("#plannedWorkingStartTime").attr("disabled",false);
                    $("#plannedWorkingEndTime").attr("disabled",false);
                    $("#changeDelayDate").val("1");
                    $("#setEndPerson").hide();
                    $("#setEndPersonContent").hide();
                    checkCheckNum();
                }else {
                    $("#resetResponsePerson").val($("#workPerson").val());
                    $("#planStartTime").val($("#plannedWorkingStartTime").val());
                    $("#planEndTime").val($("#plannedWorkingEndTime").val());
                    $("#plannedWorkingStartTime").attr("disabled",true);
                    $("#plannedWorkingEndTime").attr("disabled",true);
                    $("#changeDelayDate").val("0");
                    $("#setEndPerson").show();
                    $("#setEndPersonContent").show();
                }
            }

            function checkCheckNum(){
                var obj = document.getElementsByName('checkbox1');
                var checkNum = 0;
                if(obj){
                    for(var   i=0;   i <obj.length;   i++){
                        if(obj[i].checked   ==   true){
                            checkNum++;
                        }
                    }
                }
                if(checkNum > 1){
                    alert( "更换负责人和延期只能选择其中一种");
                    $("#changePerson").attr("checked",false);
                    $("#changeDelay").attr("checked",false);
                    $("#changeFuZeRen").val("0");
                    $("#changeDelayDate").val("0");
                    $("#setEndPerson").show();
                    $("#setEndPersonContent").show();
                }
            }

                //电气二种票隐藏增加工作审结和补充工作地点保留带电部分和安全措施部分
                var type = $("#workTicketType").val();
                if(type == "电气二种工作票"){
                    $("#addPermit").remove();
                    $("#permit").remove();
                    $("#addNeedAttentionMeasureses").remove();
                    $("#attentions").remove();
                    $("#change").remove();
                }

            var timeStart = {
                elem: '#plannedWorkingStartTime',
                format: 'YYYY-MM-DD hh:mm:ss',
                max: '2099-06-16',
                istime: true,
                istoday: true
            }
            var endStart = {
                elem: '#plannedWorkingEndTime',
                format: 'YYYY-MM-DD hh:mm:ss',
                max: '2099-06-16',
                istime: true,
                istoday: true
            }
            laydate(timeStart);
            laydate(endStart);


            function toValid(){
                //工作终结人
                if(!$("#changePerson").is(":checked") && !$("#changeDelay").is(":checked")){
                    if(!checkNull("#permitPerson","#permitNote")){
                        return false;
                    }
                }

                //校验补充工作地点保留带电部分和安全措施唯一性
                if(isRepeat("textarea[name=attentionMeasureses]")){
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

                return true;
            }
        </script>
    <%--</div>

</div>--%>

</body>

</html>