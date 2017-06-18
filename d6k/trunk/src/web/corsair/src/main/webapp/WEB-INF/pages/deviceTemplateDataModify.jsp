<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html lang="zh_CN">
<%
    session.getAttributeNames();
%>
<head>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

    <title>${sessionScope.APP_NAME}-设备模板数据编辑</title>
</head>

<body>
<div class="row">
    <div class="col-lg-12">
        <h3 class="page-header">设备模板数据编辑</h3>
    </div>
    <!-- /.col-lg-12 -->
</div>
<!-- /.row -->
<form:form cssClass="form-horizontal" action="deviceTemplateDataModify" commandName="deviceTemplateDataModifyForm">
    <form:hidden path="id"/>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
        <label class="control-label">所属模板</label>
        <span style="color:red" title="必填">(*)</span>
    </div>
        <div class="col-xs-8 col-lg-3">
            <form:select cssClass="form-control" path="deviceTemplate" items="${deviceTemplates}"
                         onblur="checkDeviceTemplateDataIndex()"/>
        </div>
    </div>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
            <label class="control-label">数据类型</label>
            <span style="color:red" title="必填">(*)</span>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:select cssClass="form-control" path="type" items="${types}" onblur="checkDeviceTemplateDataIndex()"/>
        </div>
    </div>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
            <label class="control-label">数据索引</label>
            <span style="color:red" title="必填">(*)</span>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:input type="text" cssClass="form-control" path="index" onblur="checkDeviceTemplateDataIndex()"/>
        </div>
        <span id="indexError" style="color:red"></span>
    </div>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
            <label class="control-label">数据名</label>
            <span style="color:red" title="必填">(*)</span>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:input type="text" cssClass="form-control" path="name" onblur="checkDeviceTemplateDataName()"/>
        </div>
        <span id="nameError" style="color:red"></span>
    </div>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
            <label class="control-label">描述</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:input type="text" cssClass="form-control" path="desc"/>
        </div>
    </div>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
            <label class="control-label">单位</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:input type="text" cssClass="form-control" path="unitType"/>
        </div>
    </div>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
            <label class="control-label">默认值（最小值）</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:input type="text" cssClass="form-control" path="defaultValue"
                        onblur="checkDeviceTemplateDataValue()"/>
        </div>
        <span id="defaultValueError" style="color:red"></span>
    </div>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
            <label class="control-label">最大值</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:input type="text" cssClass="form-control" path="maxValue" onblur="checkDeviceTemplateDataValue()"/>
        </div>
        <span id="maxValueError" style="color:red"></span>
    </div>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
            <label class="control-label">特殊处理(magic)</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:input type="text" cssClass="form-control" path="magic"/>
        </div>
    </div>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
            <label class="control-label">变化模式</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:select cssClass="form-control" path="changePattern" items="${changePatterns}"></form:select>
        </div>
    </div>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
            <label class="control-label">图表中是否可见</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:select cssClass="form-control" path="invisibleInChart" items="${invisibleInCharts}"></form:select>
        </div>
    </div>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
            <label class="control-label">累积类型</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:select cssClass="form-control" path="accumulateType" items="${accumulateTypes}"/>
        </div>
    </div>
    <div class="form-group">
        <div class="col-sm-2 col-lg-2">
            <label class="control-label">特殊图表</label>
        </div>
        <div class="col-xs-8 col-lg-3">
            <form:select cssClass="form-control" path="specialChart" items="${specialCharts}"/>
        </div>
    </div>
    <form:input type="submit" cssClass="btn" path="submit" value="保存"
                onclick="return checkDeviceTemplateDataModify();"/>
    <input class="btn" type="button" value="返回" onclick="history.back();"/>
</form:form>

<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

<script type="text/javascript">

    function checkDeviceTemplateDataModify() {
        with (document.forms[0]) {
            if (!index.value) {
                alert('数据索引必须填写');
                return false;
            }
            if (!name.value) {
                alert('数据名必须填写！');
                return false;
            }
        }
        document.getElementById('submit').click();
        return true;
    }

    function checkDeviceTemplateDataIndex() {
        var index = document.forms[0].index.value.trim();
        var deviceTemplate = document.forms[0].deviceTemplate.value.trim();
        var type = document.forms[0].type.value.trim();
        if (index) {
            $.ajax({
                type: 'POST',
                url: '/api/deviceTemplate/checkDeviceTemplateDataIndex',
                data: {index: index, deviceTemplate: deviceTemplate, type: type},
                success: function (response) {
                    document.getElementById('indexError').innerText = '';

                    if (response == 'indexNotNumber') {
                        document.getElementById('indexError').innerText = '数据索引必须为数字，请重新输入！';
                    } else if (response == 'isExist') {
                        document.getElementById('indexError').innerText = '该数据索引已被占用，请重新输入！';
                    } else if (response == 'ok') {
                        document.getElementById('indexError').innerText = '';
                    }
                }
            });
        }
    }

    function checkDeviceTemplateDataName() {
        var name = document.forms[0].name.value.trim();
        if (name) {
            $.ajax({
                type: 'POST',
                url: '/api/deviceTemplate/checkDeviceTemplateDataName',
                data: {name: name},
                success: function (response) {
                    document.getElementById('nameError').innerText = '';

                    if (response == '1') {
                        document.getElementById('nameError').innerText = '该数据名已被占用，请重新输入！';
                    } else if (response == '0') {
                        document.getElementById('nameError').innerText = '';
                    }
                }
            });
        }
    }

    function checkDeviceTemplateDataValue() {
        var defaultValue = document.forms[0].defaultValue.value.trim();
        var maxValue = document.forms[0].maxValue.value.trim();
        if (true) {
            $.ajax({
                type: 'POST',
                url: '/api/deviceTemplate/checkDeviceTemplateDataValue',
                data: {defaultValue: defaultValue, maxValue: maxValue},
                success: function (response) {
                    document.getElementById('defaultValueError').innerText = '';
                    document.getElementById('maxValueError').innerText = '';

                    //默认值是否为数字校验
                    if (response == 'defaultValueNotNumber') {
                        document.getElementById('defaultValueError').innerText = '默认值必须为数字，请重新输入！';
                    }

                    //最大值是否为数字校验
                    if (response == 'maxValueNotNumber') {
                        document.getElementById('maxValueError').innerText = '最大值必须为数字，请重新输入！';
                    }

                    //最大值合理性校验
                    if (response == 'maxValueError') {
                        document.getElementById('maxValueError').innerText = '最大值不能小于默认值，请重新输入！';
                    }
                }

            });
        }
    }
</script>
</body>
</html>
