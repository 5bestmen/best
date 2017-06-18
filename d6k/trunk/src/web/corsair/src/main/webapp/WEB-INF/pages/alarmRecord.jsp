<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html>
<head>
    <title>${sessionScope.APP_NAME}-告警查询</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/multiSelect/sol.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/alertify.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/multiSelect/bootstrap-multiselect.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sweetalert.min.css" />" rel="stylesheet">

    <%
        // 获取请求的上下文
        String context = request.getContextPath();


    %>

    <style type="text/css">
        ul {
            list-style: none;
            margin: 0px;
            padding: 0;
        }

        .label {
            color: black;
            text-align: left;
            font-size: 14px;
        }

        .sol-label-text {
            color: #ffffff;
        }

        /*.btn{
            background-color: #ffffff;
            color: #000000;
        }*/
        .multiselect-clear-filter {
            padding: 9px 12px;
        }

        .query {
            margin-top: 20px;
        }

        /*td*/
        #mytable td {
            color: #ffffff;
            padding-bottom: 10px;
        }

        .form-group {
            margin: 5px 0px;
        }

        .col-xs-3 {
            padding-left: 0px;
            width: 25%;
        }

        .table-menu > td
        {
            color: #ffffff;
            width: 100px;
            white-space: nowrap;
            padding-left: 2px;
            padding-right: 2px;
        }

        .table-data > td
        {
            color: orange;
            width: 100px;
            border-width: 1px;
            white-space: nowrap;
            padding-left: 2px;
            padding-right: 2px;
        }
        .ReportArea
        {
            overflow: auto;
        }


        #monthReportDate {
            color: #000000;
        }

        .col-xs-4 {
            width: 32%;
        }

        .multiselect {
            background-color: #ffffff;
            color: #555;
        }

        /*.laydate_body .laydate_table td {*/
        /*color: black;*/
        /*}*/

        .btn-group {
            float: right;
        }

        .laydate-icon {
            background-image: url(/resources/img/Calendar.png);
        }

        /*.dropdown-menu, .dropdown-menu > li > a:hover, .dropdown-menu > li > a {*/
        /*background-color: rgb(31, 86, 134);*/
        /*}*/
    </style>
</head>

<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/jquery.table2excel.js"/>"></script>
<script src="<c:url value="/resources/dist/laydate/laydate.js"/>"></script>
<script src="<c:url value="/resources/js/multiSelect/toc.js"/>"></script>
<script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
<script src="<c:url value="/resources/js/multiSelect/sol.js"/>"></script>
<script src="<c:url value="/resources/js/multiSelect/bootstrap-multiselect.js"/>"></script>
<script src="<c:url value="/resources/js/generalChart.js"/>"></script>
<script src="<c:url value="/resources/js/alertify.min.js"/>"></script>
<script src="<c:url value="/resources/js/sweetalert.min.js" />"></script>


<script>
    Date.prototype.format = function (fmt) {
        var o = {
            "M+": this.getMonth() + 1, //月份
            "d+": this.getDate(), //日
            "h+": this.getHours(), //小时
            "m+": this.getMinutes(), //分
            "s+": this.getSeconds(), //秒
            "q+": Math.floor((this.getMonth() + 3) / 3), //季度
            "S": this.getMilliseconds() //毫秒
        };
        if (/(y+)/.test(fmt)) fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
        for (var k in o)
            if (new RegExp("(" + k + ")").test(fmt)) fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));
        return fmt;
    };

    function timeInit() {
        var beginDate, endDate,chooseDta;
        beginDate = laydate.now(0, "YYYY-MM-DD");
        endDate = laydate.now(0, "YYYY-MM-DD");
        chooseDta = laydate.now(0, "YYYY-MM-DD");
        $('#beginDate').val(beginDate);
        $('#endDate').val(endDate);
        $('#chooseDate').val(chooseDta);
    }

    var count = 0;
    function add() {
        var str = '';
        str += '<tr>';
        str += '<td>站点</td>';
        str += '<td class="col-xs-2"><select name="station" class="form-control"><option>请选择</option>';
        str += '</select></td>';
        str += '<td>设备</td>';
        str += '<td class="col-xs-2"><select name="device" class="form-control"><option>请选择</option>';
        str += '</select></td>';
        str += '<td>测点</td>';
        str += '<td class="col-xs-7" style="width: 53%"><select name="example-selectAllJustVisible" multiple="multiple">';
        str += '</select></td>';
        str += '<td><button type="button" class="btn btn-primary btn-sm" name="delete" >删除</button></td>';
        str += '</tr>';
        $("#mytable").append(str);
        count++;
        var $select = $(event.target).parent().parent().parent().parent().find("tr").find("td:eq(1)").children().eq(count);
        deleteTr();
        init($select);
        showEquipments();
        styleInit();
        showDevices();
    }

    function init(obj) {
        $.ajax({
            type: "GET",
            url: "/api/psr/subStations",
            contentType: "application/json",
            dataType: "json",
            async: false,
            success: function (data) {
                result = data;
                for (var i = 0; i < result.length; i++) {
                    obj.append("<option value='" + result[i].id + "'>" + result [i].name + "</option>");
                }
            }
        });
    }

    function deleteTr() {
        $("button[name=delete]").click(function (event) {
            count--;
            $(event.target).parent().parent().remove();
        })
    }

    function findParentElement(element) {
        return $(element).parent();
    }

    function findNextElement(element) {
        return $(element).parent().next().next().children();
    }

    function getNotRepeatingUnit(new_arr, templates) {
        for (var i = 0; i < templates.length; i++) {
            var items = templates.eq(i).attr("unit");
            if ($.inArray(items, new_arr) == -1) {
                new_arr.push(items);
            }
        }
    }

    var alarmType;
    var alarmName;
    function selectAlarmType(id, name){
        alarmType = id;
        alarmName = name;
    }

    $(function () {

        $('[data-toggle="tooltip"]').tooltip();

        init($("select[name=station]"));

        timeInit();

        var beginDate = {
            elem: '#beginDate',
            format: 'YYYY-MM-DD',
            istime: true,
            istoday: true
        };

        var endDate = {
            elem: '#endDate',
            format: 'YYYY-MM-DD',
            istime: true,
            istoday: true
        };

        var chooseDate = {
            elem: '#chooseDate',
            format: 'YYYY-MM-DD',
            istime: true,
            istoday: true
        };

        laydate(beginDate);
        laydate(endDate);
        laydate(chooseDate);

        $('#today').on('click', function () {
            timeInit();
        });

        $('#prev').on('click', function () {
            var beginFlag = document.getElementById("begin_radio").checked;
            var chooseFlag = document.getElementById("choose_radio").checked;
            if (beginFlag) {
                var beginDate = new Date(Date.parse($('#beginDate').val().replace(/-/g, "/")));
                beginDate.setDate(beginDate.getDate() - 1);
                $('#beginDate').val(beginDate.format("yyyy-MM-dd"));
            } else if (chooseFlag) {
                var chooseDate = new Date(Date.parse($('#chooseDate').val().replace(/-/g, "/")));
                chooseDate.setDate(chooseDate.getDate() - 1);
                $('#chooseDate').val(chooseDate.format("yyyy-MM-dd"));
            }
        });

        $('#next').on('click', function () {
            var beginFlag = document.getElementById("begin_radio").checked;
            var chooseFlag = document.getElementById("choose_radio").checked;

            var beginDate = new Date(Date.parse($('#beginDate').val().replace(/-/g, "/")));
            var chooseDate = new Date(Date.parse($('#chooseDate').val().replace(/-/g, "/")));
            var today = new Date();

            if (beginFlag) {
                if (beginDate.format("yyyy-MM-dd") == today.format("yyyy-MM-dd")) {
                    swal({
                        title: "不能查询未来的数据!",
                        text: "不能查询未来的数据!！",
                        confirmButtonText: "确定"
                    });
                    return;
                }

                beginDate.setDate(beginDate.getDate() + 1);
                $('#beginDate').val(beginDate.format("yyyy-MM-dd"));
            }  else if (chooseFlag) {
                if (chooseDate.format("yyyy-MM-dd") == today.format("yyyy-MM-dd")) {
                    swal({
                        title: "不能查询未来的数据!",
                        text: "不能查询未来的数据!！",
                        confirmButtonText: "确定"
                    });
                    return;
                }

                chooseDate.setDate(chooseDate.getDate() + 1);
                $('#chooseDate').val(chooseDate.format("yyyy-MM-dd"));
            }
        });

        $("#query").click(function () {

            /*var type = parent.getTreeType();
            if (type != 4) {
                swal({
                    title: "重新选择！",
                    text: "请选择左侧树形结构再查询！",
                    confirmButtonText: "确定"
                });
                return;
            }*/

            //alert(alarmType);
            queryAlarm(1);

        });

    });


//报表导出
    $(function(){

        $("#export2excel").click(function(){
            $(".ReportArea").table2excel({

                // 导出的Excel文档的名称
                name: "Excel Document Name",

                // todo:Excel文件的名称（传入）
                filename: "事件记录(" + alarmName + ")"
                //filename: fileName
            })
        })

    });

    function queryAlarm(currentPage) {
        switch (alarmType) {
            case "allAlarm":
                break;
            case "switchAlarm":
                break;
            case "measureAlarm":
                break;
            case "operationAlarm":
                break;
            default :
                swal({
                    title: "请选择告警类型!",
                    text: "请选择告警类型!",
                    confirmButtonText: "确定"
                });
                return;
        }

        if (currentPage <=0) {
            return;
        }

        var stationID = parent.getTreeStationID();
        var stationName = parent.getTreeStationName();
        var deviceID = parent.getTreeDeviceID();
        var deviceName = parent.getTreeText();

        var chooseName = parent.getTreeText();
        var level = parent.getTreeLevel();

        var option = {
            stationId: stationID,
            deviceId: deviceID,
            stationName: stationName,
            deviceName: deviceName,
            level: level,
            chooseName: chooseName
        };


        var name = alarmName;

        //日报表或月报表选择
        var beginFlag = document.getElementById("begin_radio").checked;
        var chooseFlag = document.getElementById("choose_radio").checked;
        if (beginFlag) {
            var beginDate = $("#beginDate").val();
            var endDate = $("#endDate").val();

            var data = {
                type: 1, // 1：表示间隔查询
                alarmType: alarmType,
                name: name,
                templates: option,
                beginDate: beginDate,
                endDate: endDate
            };

            queryAlarmInterval(data, currentPage);
        } else if (chooseFlag) {
            var chooseDate = $("#chooseDate").val();

            var data = {
                type: 2, // 2:某日查询
                alarmType: alarmType,
                name: name,
                templates: option,
                chooseDate: chooseDate
            };

            queryAlarmDay(data, currentPage);
        }
    }

    // 告警查询函数
    function queryAlarmInterval(data, currentPage) {

        //判断时间合理性
        var today = new Date();
        if ((data.beginDate.format("yyyy-MM-dd") > today.format("yyyy-MM-dd")) ||
                (data.endDate.format("yyyy-MM-dd") > today.format("yyyy-MM-dd")) ||
                (data.beginDate.format("yyyy-MM-dd") > data.endDate.format("yyyy-MM-dd"))
        ) {
            swal({
                title: "选择的时间有误，请重新选择!",
                text: "选择的时间有误，请重新选择!",
                confirmButtonText: "确定"
            });
            return;
        }

        var tableObj = $('#searchID');
        tableObj.empty();

        var head = "<br/><br/><br/><br/><div class=\"tabTopBar\"><br/>";
        head += "<div class=\"ttbTxt\" align=\"center\"><h2>" + data.name + "</h2></div>";
        head += "</div>";
        head += "<div class = 'ReportArea'>";
        head += "<table id='dayData' border=\"1\" bordercolor='grey'>";
        head += "<thead>";
        head += "<tr style='font-weight : bold' class='table-menu'> <td colspan='3' style='text-align:center;'>" + "事件记录(" + data.name + ")" + "</td></tr>";
        head += "<tr style='font-weight : bold' class=\"table-menu\">";
        head += "<td style='text-align:center;'>时间</td>";
        head += "<td style='text-align:center;'>描述</td>";
        head += "<td style='text-align:center;'>类型</td>";
        head += "</tr>";
        head += "</thead>";
        head += "<tbody>";
        head += "</tbody>";
        head += "</table>";
        head += "</div>";

        tableObj.append(head);

        $.ajax({
            type: "POST",
            url: "/api/multiCharts/alarmRecordInterval",
            data: {
                stationId: data.templates.stationId,
                deviceId: data.templates.deviceId,
                beginDate: data.beginDate.format("yyyy-MM-dd"),
                endDate: data.endDate.format("yyyy-MM-dd"),
                alarmType: data.alarmType,
                currentPage: currentPage,
                level: data.templates.level,
                chooseName: data.templates.chooseName
            },

            success: function (result) {
//                var dataObj = jQuery.parseJSON(result);

                if (result.dataList.length <= 0)
                {
                    swal({
                        title: "数据库中未查询到数据!",
                        text: "数据库中没有存储数据!",
                        confirmButtonText: "确定"
                    });

                    tableObj.empty();

                    var pagerObj = $('#pagerOfAlarm');
                    pagerObj.empty();
                    return;
                }

                var strAlarmType;
                var tableObj1 = $('#dayData');
                for (var i = 0; i < result.dataList.length; i++) {
                    var tr = "<tr class=\"table-data\">";
                    // 测点的值
                    tr += "<td style='text-align:center;'>" + result.dataList[i].time + "</td>";
                    tr += "<td style='text-align:center;'>" + result.dataList[i].describe + "</td>";
                    switch (result.dataList[i].type) {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                            strAlarmType = "遥测越限";
                            break;
                        case 100:
                            strAlarmType = "开关变位";
                            break;
                        case 200:
                            strAlarmType = "操作事项";
                            break;
                        default:
                            continue;
                    }
                    tr += "<td style='text-align:center;'>" + strAlarmType + "</td>";

                    tr += "</tr>";
                    tableObj1.append(tr);
                }

                // pagerOfAlarm
                var pagerObj = $('#pagerOfAlarm');
                pagerObj.empty();

                g_currentPage = result.currentPage;
                g_totalPage = result.totalPage;

                var pageInfo = "共";
                pageInfo += result.totalRecord;
                pageInfo += "条记录共";
                pageInfo += result.totalPage;
                pageInfo += "页  当前第";
                pageInfo += "<span style=\"color:red\">";
                pageInfo += result.currentPage;
                pageInfo += "</span>";
                pageInfo += "页";
                var trTmp;
                trTmp = "<div float=\"right\">";
                trTmp += pageInfo;
                trTmp += "<span>  </span>";
                trTmp += "<a href=\"#\" onclick=\"firstPage();\" style=\"background-color:orangered;\">首页</a>";
                trTmp += "<span>  </span>";
                trTmp += "<a href=\"#\" onclick=\"previousPage();\" style=\"background-color:orangered;\">  上一页</a>";
                trTmp += "<span>  </span>";
                trTmp += "<a href=\"#\" onclick=\"nextPage();\" style=\"background-color:orangered;\">  下一页</a>";
                trTmp += "<span>  </span>";
                trTmp += "<a href=\"#\" onclick=\"lastPage();\" style=\"background-color:orangered;\">  尾页</a>";
                trTmp += "<span>  </span>";
                trTmp += "<input id=\"choosePage\" type=\"text\" value=";
                trTmp += result.currentPage;
                trTmp += " style=\"width:35px; color:red \">";
                trTmp += "<span>  </span>";
                trTmp += "<a href=\"#\" onclick=\"surePage();\" style=\"background-color:orangered;\">确定</a>";
                trTmp += "</div>";

                pagerObj.append(trTmp);
            },
            error: function (errorMsg) {
                swal({
                    title: "不好意思，图表请求数据失败啦!",
                    text: "不好意思，图表请求数据失败啦!",
                            confirmButtonText: "确定"
                });
            }
        });

    }

    // 告警查询函数
    function queryAlarmDay(data, currentPage) {

        //判断时间合理性
        var today = new Date();
        if (data.chooseDate.format("yyyy-MM-dd") > today.format("yyyy-MM-dd")) {
            swal({
                title: "选择的时间有误，请重新选择!",
                text: "选择的时间有误，请重新选择!",
                confirmButtonText: "确定"
            });
            return;
        }

        var tableObj = $('#searchID');
        tableObj.empty();

        var head = "<br/><br/><br/><br/><div class=\"tabTopBar\"><br/>";
        head += "<div class=\"ttbTxt\" align=\"center\"><h2>" + data.name + "</h2></div>";
        head += "</div>";
        head += "<div class = 'ReportArea'>";
        head += "<table id='dayData' border=\"1\" bordercolor='grey'>";
        head += "<thead>";
        head += "<tr style='font-weight : bold' class='table-menu'> <td colspan='3' style='text-align:center;'>" + "事件记录(" + data.name + ")" + "</td></tr>";
        head += "<tr style='font-weight : bold' class=\"table-menu\">";
        head += "<td style='text-align:center;'>时间</td>";
        head += "<td style='text-align:center;'>描述</td>";
        head += "<td style='text-align:center;'>类型</td>";
        head += "</tr>";
        head += "</thead>";
        head += "<tbody>";
        head += "</tbody>";
        head += "</table>";
        head += "</div>";

        tableObj.append(head);

        $.ajax({
            type: "POST",
            url: "/api/multiCharts/alarmRecordDay",
            data: {
                stationId: data.templates.stationId,
                deviceId: data.templates.deviceId,
                date: data.chooseDate.format("yyyy-MM-dd"),
                alarmType: data.alarmType,
                currentPage: currentPage,
                level: data.templates.level,
                chooseName: data.templates.chooseName
            },

            success: function (result) {
                //var result = jQuery.parseJSON(data);

                if (result.dataList.length == 0)
                {
                    swal({
                        title: "数据库中未查询到数据!",
                        text: "数据库中没有存储数据!",
                        confirmButtonText: "确定"
                    });

                    tableObj.empty();

                    var pagerObj = $('#pagerOfAlarm');
                    pagerObj.empty();

                    return;
                }

                var strAlarmType;
                var tableObj1 = $('#dayData');

                for (var i = 0; i < result.dataList.length; i++) {
                    var tr = "<tr class=\"table-data\">";
                    // 测点的值
                    tr += "<td style='text-align:center;'>" + result.dataList[i].time + "</td>";
                    tr += "<td style='text-align:center;'>" + result.dataList[i].describe + "</td>";
                    switch (result.dataList[i].type) {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                            strAlarmType = "遥测越限";
                            break;
                        case 100:
                            strAlarmType = "开关变位";
                            break;
                        case 200:
                            strAlarmType = "操作事项";
                            break;
                        default:
                            continue;
                    }
                    tr += "<td style='text-align:center;'>" + strAlarmType + "</td>";

                    tr += "</tr>";

                    tableObj1.append(tr);
                }

                // pagerOfAlarm
                var pagerObj = $('#pagerOfAlarm');
                pagerObj.empty();

                g_currentPage = result.currentPage;
                g_totalPage = result.totalPage;

                var pageInfo = "共";
                pageInfo += result.totalRecord;
                pageInfo += "条记录共";
                pageInfo += result.totalPage;
                pageInfo += "页  当前第";
                pageInfo += "<span style=\"color:red\">";
                pageInfo += result.currentPage;
                pageInfo += "</span>";
                pageInfo += "页";
                var trTmp;
                trTmp = "<div float=\"right\">";
                trTmp += pageInfo;
                trTmp += "<span>  </span>";
                trTmp += "<a href=\"#\" onclick=\"firstPage();\" style=\"background-color:orangered;\">首页</a>";
                trTmp += "<span>  </span>";
                trTmp += "<a href=\"#\" onclick=\"previousPage();\" style=\"background-color:orangered;\">  上一页</a>";
                trTmp += "<span>  </span>";
                trTmp += "<a href=\"#\" onclick=\"nextPage();\" style=\"background-color:orangered;\">  下一页</a>";
                trTmp += "<span>  </span>";
                trTmp += "<a href=\"#\" onclick=\"lastPage();\" style=\"background-color:orangered;\">  尾页</a>";
                trTmp += "<span>  </span>";
                trTmp += "<input id=\"choosePage\" type=\"text\" value=";
                trTmp += result.currentPage;
                trTmp += " style=\"width:35px; color:red \">";
                trTmp += "<span>  </span>";
                trTmp += "<a href=\"#\" onclick=\"surePage();\" style=\"background-color:orangered;\">确定</a>";
                trTmp += "</div>";

                pagerObj.append(trTmp);
            },
            error: function (errorMsg) {
                swal({
                    title: "不好意思，图表请求数据失败啦!",
                    text: "不好意思，图表请求数据失败啦!",
                    confirmButtonText: "确定"
                });
            }
        });

    }

    function GetDays(Year,Month)
    {
        return new Date(Year,Month,0).getDate();
    }

</script>

<script>
    var g_currentPage;
    var g_totalPage;
    // 第一页
    function firstPage(){

        if(g_currentPage == 1){
            return false;
        }else{
            queryAlarm(1);
            return true;
        }
    }

    // 上一页
    function previousPage(){
        if(g_currentPage == 1){
            return false;
        }else{
            queryAlarm(g_currentPage-1);
            return true;
        }
    }

    // 下一页
    function nextPage(){
        if(g_currentPage == g_totalPage){
            return false;
        }else{
            queryAlarm(g_currentPage+1);
            return true;
        }
    }

    // 尾页
    function lastPage(){
        if(g_currentPage == g_totalPage){
            return false;
        }else{
            queryAlarm(g_totalPage);
            return true;
        }
    }

    // 跳转
    function surePage(){
        var choosePage = $('#choosePage').val();
        var re = /^[1-9]+[0-9]*]*$/;
        if (!re.test(choosePage)) {
            return false;
        }
        if(g_currentPage == choosePage){
            return false;
        }else if (choosePage > g_totalPage){
            document.getElementById("choosePage").value=g_totalPage;
            queryAlarm(g_totalPage);
            return true;
        } else {
            document.getElementById("choosePage").value=choosePage;
            queryAlarm(choosePage);
            return true;
        }
    }

</script>

<div id="content" class="clear">
    <form class="form-horizontal">
        <div class="col-md-12 query" style="background: #3298d2">
            <div id="change"></div>
            <div class="form-group">
                <div class="col-xs-3">
                    <div class="input-group">
                        <input type="radio" id="begin_radio" name="query_radio" theme="simple" checked="true"/>
                        <span class="input-group-addon">开始日期</span>

                        <div class="input-group date form_date">
                            <input class="form-control laydate-icon layer-date" id="beginDate"/>
                        </div>
                    </div>
                </div>
                <div class="col-xs-3">
                    <div class="input-group">
                        <span class="input-group-addon">结束日期</span>
                        <div class="input-group date form_date">
                            <input class="form-control laydate-icon layer-date" id="endDate"/>
                        </div>
                    </div>
                </div>

                <div style="float: left">
                    <select onchange="selectAlarmType(this.options[selectedIndex].id, this.options[selectedIndex].value)" name="alarmType" class="form-control">
                        <option id="head" style="background-color:blue;color:black" selected>告警类型选择</option>
                        <option id="allAlarm" style="background-color:yellow;color:black">全部告警</option>
                        <option id="switchAlarm" style="background-color:orangered;color:black">开关变位</option>
                        <option id="measureAlarm" style="background-color:yellow;color:black">遥测越限</option>
                        <option id="operationAlarm" style="background-color:orangered;color:black">操作事项</option>
                    </select>
                </div>

                <div class="col-xs-3">
                    <div class="btn-group" role="group">
                        <button type="button" data-toggle="tooltip" data-placement="left" title=""
                                data-original-title="前一天" class="btn btn-primary glyphicon glyphicon-arrow-left"
                                id="prev"></button>
                        <button type="button" data-toggle="tooltip" data-placement="top" title=""
                                data-original-title="今天" class="btn btn-primary glyphicon glyphicon-pushpin"
                                id="today"></button>
                        <button type="button" data-toggle="tooltip" data-placement="bottom" title=""
                                data-original-title="后一天" class="btn btn-primary glyphicon glyphicon-arrow-right"
                                id="next"></button>
                        <button type="button" data-toggle="tooltip" data-placement="right" title=""
                                data-original-title="查询" class="btn btn-primary glyphicon glyphicon-search"
                                id="query"></button>
                        <button type="button" data-toggle="tooltip" data-placement="right" title=""
                                data-original-title="导出" class="btn btn-primary glyphicon glyphicon-save"
                                id="export2excel" ></button>
                                <%--id="export2excel" onclick="exportData()" ></button>--%>
                    </div>
                </div>
            </div>
            <div class="form-group">
                <div class="col-xs-3">
                    <div class="input-group">
                        <input type="radio" id="choose_radio" name="query_radio" theme="simple" checked="true"/>
                        <span class="input-group-addon">选择日期</span>

                        <div class="input-group date form_date">
                            <input class="form-control laydate-icon layer-date" id="chooseDate"/>
                        </div>
                    </div>
                </div>
            </div>

        </div>
    </form>
    <p></p>

    <p></p>
    <!--绘制报表-->
    <div style="float:none" align="center">
        <div id="searchID">


        </div>
        <br>

        <div id="pagerOfAlarm" style="float:right;margin-bottom:3px;">

        </div>
        <%--<div float="right">--%>
            <%--共0条记录共0页  当前第0页--%>
            <%--<a href="#" onclick="firstPage();">首页</a>--%>
            <%--<a href="#" onclick="nextPage();">下一页</a>--%>
            <%--<a href="#" onclick="previousPage();">上一页</a>--%>
            <%--<a href="#" onblur="lastPage();">尾页</a>--%>
        <%--</div>--%>
        <br>
    </div>

</div>
</body>
</html>
