<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html>
<head>
    <title>${sessionScope.APP_NAME}-报表查询</title>
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
            width: 30%;
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
        var dayReportDate, monthReportDate;
        dayReportDate = laydate.now(0, "YYYY-MM-DD");
        monthReportDate = laydate.now(0, "YYYY-MM");
        $('#dayReportDate').val(dayReportDate);
        $('#monthReportDate').val(monthReportDate);
    }

    function styleInit() {
        $("select[name=example-selectAllJustVisible]").multiselect({
            buttonWidth: '100%',
            buttonText: function (options) {
                if (options.length == 0) {
                    return '无选择';
                }
            }
        });
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

    $(function () {

        $('[data-toggle="tooltip"]').tooltip();

        init($("select[name=station]"));
        styleInit();
        timeInit();

        var dayReportDate = {
            elem: '#dayReportDate',
            format: 'YYYY-MM-DD',
            istime: true,
            istoday: true
        };

        var monthReportDate = {
            elem: '#monthReportDate',
            format: 'YYYY-MM',
            istime: true,
            istoday: true
        };

        laydate(dayReportDate);
        laydate(monthReportDate);

        $('#today').on('click', function () {
            timeInit();
        });

        $('#prev').on('click', function () {
            var dayReportDate = new Date(Date.parse($('#dayReportDate').val().replace(/-/g, "/")));
            dayReportDate.setDate(dayReportDate.getDate() - 1);
            $('#dayReportDate').val(dayReportDate.format("yyyy-MM-dd"));
        });

        $('#next').on('click', function () {
            var dayReportDate = new Date(Date.parse($('#dayReportDate').val().replace(/-/g, "/")));
            var today = new Date();

            if (dayReportDate.format("yyyy-MM-dd") == today.format("yyyy-MM-dd")) {
                swal({
                    title: "不能查询未来的数据!",
                    text: "不能查询未来的数据!！",
                    confirmButtonText: "确定"
                });
                return;
            }

            dayReportDate.setDate(dayReportDate.getDate() + 1);
            $('#dayReportDate').val(dayReportDate.format("yyyy-MM-dd"));
        });

        $("#query").click(function () {

            var level = parent.getTreeLevel();
            if (level != 4) {
                swal({
                    title: "请先选择设备再查询！",
                    text: "请先选择设备再查询！",
                    confirmButtonText: "确定"
                });
                return;
            }

            var stationID = parent.getTreeStationID();
            var stationName = parent.getTreeStationName();
            var deviceID = parent.getTreeDeviceID();
            var deviceName = parent.getTreeText();

            var option = {
                stationId: stationID,
                deviceId: deviceID,
                stationName: stationName,
                deviceName: deviceName
            };


            //日报表或月报表选择
            var dayFlag = document.getElementById("dayreport_radio").checked;
            var monthFlag = document.getElementById("monthreport_radio").checked;
            if (dayFlag) {
                var dayReportDate = $("#dayReportDate").val();

                var data = {

                    templates: option,
                    queryDate: dayReportDate
                };

                queryDayReport(data);
            } else if (monthFlag) {
                var monthReportDate = $("#monthReportDate").val();

                var data = {

                    templates: option,
                    queryDate: monthReportDate
                };
                queryMonthReport(data);
            }

        });

    });


//报表导出
    $(function(){
//        if(data.queryDate.length > 8){
//            var fileName = data.templates[0].deviceName + "-" + data.queryDate + "月报表";
//        }else
//        {
//            var fileName = data.templates[0].deviceName + "-" + data.queryDate + "日报表";
//        }

        $("#export2excel").click(function(){
            var deviceName = parent.getTreeText();
            var fileName;
            var dayFlag = document.getElementById("dayreport_radio").checked;
            if (dayFlag) {
                fileName = "日报表("+deviceName+")";
            } else {
                fileName = "月报表("+deviceName+")";
            }
            $(".ReportArea").table2excel({

                // 导出的Excel文档的名称
                name: "Excel Document Name",

                // todo:Excel文件的名称（传入）
                filename: fileName
            })
        })

    });

    //日报表查询函数
    function queryDayReport(data) {
        //判断时间合理性
        var today = new Date();
        if (data.queryDate.format("yyyy-MM-dd") > today.format("yyyy-MM-dd")) {
            swal({
                title: "日报表查询日期必须小于今天!",
                text: "日报表查询时间应该在今天之前!",
                confirmButtonText: "确定"
            });
            return;
        }

        var tableObj = $('#searchID');
        tableObj.empty();

        var head = "<br/><br/><div class=\"tabTopBar\"><br/>";
        head += "<div class=\"ttbTxt\" align=\"center\"><h2>日报表数据查询</h2><br/></div>";
        head += "</div>";
        head += "<div class = 'ReportArea'>";
        head += "<table id='dayData' border=\"1\" bordercolor='grey'>";
        head += "<caption style='color: #d3d3d3'>报表查询时间：【" + data.queryDate + "】站点：【" + data.templates.stationName + "】设备：【" + data.templates.deviceName +"】</caption>";
        head += "<thead>";
        head += "<tr style='font-weight : bold' class='table-menu'> <td colspan='26' style='text-align:center;'>" + "日报表(" + data.templates.deviceName + "—" + data.queryDate + ")" + "</td></tr>";
        head += "<tr style='font-weight : bold' class=\"table-menu\">";
        head += "<td style='text-align:center;'>测点\\时间</td>";
        head += "<td style='text-align:center;'>0:00</td>";
        head += "<td style='text-align:center;'>1:00</td>";
        head += "<td style='text-align:center;'>2:00</td>";
        head += "<td style='text-align:center;'>3:00</td>";
        head += "<td style='text-align:center;'>4:00</td>";
        head += "<td style='text-align:center;'>5:00</td>";
        head += "<td style='text-align:center;'>6:00</td>";
        head += "<td style='text-align:center;'>7:00</td>";
        head += "<td style='text-align:center;'>8:00</td>";
        head += "<td style='text-align:center;'>9:00</td>";
        head += "<td style='text-align:center;'>10:00</td>";
        head += "<td style='text-align:center;'>11:00</td>";
        head += "<td style='text-align:center;'>12:00</td>";
        head += "<td style='text-align:center;'>13:00</td>";
        head += "<td style='text-align:center;'>14:00</td>";
        head += "<td style='text-align:center;'>15:00</td>";
        head += "<td style='text-align:center;'>16:00</td>";
        head += "<td style='text-align:center;'>17:00</td>";
        head += "<td style='text-align:center;'>18:00</td>";
        head += "<td style='text-align:center;'>19:00</td>";
        head += "<td style='text-align:center;'>20:00</td>";
        head += "<td style='text-align:center;'>21:00</td>";
        head += "<td style='text-align:center;'>22:00</td>";
        head += "<td style='text-align:center;'>23:00</td>";
        head += "<td style='text-align:center;'>单位</td>";
        head += "</tr>";
        head += "</thead>";
        head += "<tbody>";
        head += "</tbody>";
        head += "</table>";
        head += "</div>";

        tableObj.append(head);

        $.ajax({
            type: "POST",
            url: "/api/multiCharts/dayReport",
            data: {
                stationId: data.templates.stationId,
                deviceId: data.templates.deviceId,
                date: data.queryDate.format("yyyy-MM-dd")
            },

            success: function (result) {
//                var dataObj = jQuery.parseJSON(result);

                if (result.length <= 0)
                {
                    swal({
                        title: "数据库中未查询到数据!",
                        text: "数据库中没有存储数据!",
                        confirmButtonText: "确定"
                    });
                    return;
                }


                var tableObj1 = $('#dayData');
                for (var i = 0; i < result.length; i++) {
                    var tr = "<tr class=\"table-data\">";
                    // 测点的值
                    tr += "<td style='text-align:center;'>" + result[i].name + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value0 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value1 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value2 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value3 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value4 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value5 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value6 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value7 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value8 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value9 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value10 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value11 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value12 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value13 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value14 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value15 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value16 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value17 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value18 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value19 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value20 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value21 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value22 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].value23 + "</td>";
                    tr += "<td style='text-align:center;'>" + result[i].unit + "</td>";

                    tr += "</tr>";
                    tableObj1.append(tr);
                }
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

    //月报表查询函数
    function queryMonthReport(data) {
        //判断时间合理性
        var today = new Date();
        var mth = data.queryDate.format("yyyy-MM");
        var year = mth.substring(0,4);
        var month = mth.substring(5,7);

        if (mth > today.format("yyyy-MM")) {
            swal({
                title: "月报表查询日期必须小于或等于当月!",
                text: "月报表查询时间不能超过本月!",
                confirmButtonText: "确定"
            });
            return;
        }

        var days = GetDays(year, month);
        var tableObj = $('#searchID');
        tableObj.empty();

        var head1 = "<br/><br/><div class=\"tabTopBar\"><br/>";
        head1 += "<div class=\"ttbTxt\" align=\"center\"><h2>月报表数据查询</h2><br/></div>";
        head1 += "</div>";
        head1 += "<div class = 'ReportArea'>";
        head1 += "<table id=\"monthData\" border=\"1\" bordercolor='grey'>";
        head1 += "<caption style='color: #d3d3d3'>报表时间：【" + data.queryDate + "】站点：【" + data.templates.stationName + "】设备：【" + data.templates.deviceName +"】</caption>";
        head1 += "<thead>";
        head1 += "<tr class='table-menu' style='font-weight : bold'> <td colspan='"+ (days+2) +"' style='text-align:center;'>" + "月报表(" + data.templates.deviceName + "—" + data.queryDate + ")" + "</td></tr>";
        head1 += "<tr style='font-weight: bold' nowrap=\"nowrap\" class='table-menu'>";
        head1 += "<td style='text-align:center;'>测点\\月份</td>";
        for(var d = 1; d <= days; d++){
            head1 += "<td style='text-align:center;'>" + d + "</td>";
        }
        head1 += "<td style='text-align:center;'>单位</td>";
        head1 += "</tr>";
        head1 += "</thead>";
        head1 += "<tbody>";
        head1 += "</tbody>";
        head1 += "</table>";
        head1 += "</div>";

        tableObj.append(head1);

        $.ajax({
            type: "POST",
            url: "/api/multiCharts/monthReport",
            data:  {
                stationId: data.templates.stationId,
                deviceId: data.templates.deviceId,
                date: mth
            },

            success: function (result) {

                if (result.length <= 0)
                {
                    swal({
                        title: "数据库中未查询到数据!",
                        text: "数据库中没有存储该月份该点的数据!",
                        confirmButtonText: "确定"
                    });
                    return;
                }

                var tableObj1 = $('#monthData');

                for (var i = 0; i < result.length; i++) {

                    var tr = "<tr class=\"table-data\">";
                    tr += "<td style='text-align:center;'>" + result[i].name + "</td>";

                    for (var j = 1; j <= days; j++) {

                        switch (j) {
                            case 1:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value1 + "</td>";
                                break;
                            case 2:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value2 + "</td>";
                                break;
                            case 3:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value3 + "</td>";
                                break;
                            case 4:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value4 + "</td>";
                                break;
                            case 5:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value5 + "</td>";
                                break;
                            case 6:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value6 + "</td>";
                                break;
                            case 7:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value7 + "</td>";
                                break;
                            case 8:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value8 + "</td>";
                                break;
                            case 9:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value9 + "</td>";
                                break;
                            case 10:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value10 + "</td>";
                                break;
                            case 11:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value11 + "</td>";
                                break;
                            case 12:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value12 + "</td>";
                                break;
                            case 13:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value13 + "</td>";
                                break;
                            case 14:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value14 + "</td>";
                                break;
                            case 15:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value15 + "</td>";
                                break;
                            case 16:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value16 + "</td>";
                                break;
                            case 17:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value17 + "</td>";
                                break;
                            case 18:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value18 + "</td>";
                                break;
                            case 19:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value19 + "</td>";
                                break;
                            case 20:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value20 + "</td>";
                                break;
                            case 21:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value21 + "</td>";
                                break;
                            case 22:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value22 + "</td>";
                                break;
                            case 23:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value23 + "</td>";
                                break;
                            case 24:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value24 + "</td>";
                                break;
                            case 25:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value25 + "</td>";
                                break;
                            case 26:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value26 + "</td>";
                                break;
                            case 27:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value27 + "</td>";
                                break;
                            case 28:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value28 + "</td>";
                                break;
                            case 29:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value29 + "</td>";
                                break;
                            case 30:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value30 + "</td>";
                                break;
                            case 31:
                                // 测点的值
                                tr += "<td style='text-align:center;'>" + result[i].value31 + "</td>";
                                break;
                        }

                    }

                    tr += "<td style='text-align:center'>" + result[i].unit + "</td>";
                    tr += "</tr>";

                    tableObj1.append(tr);
                }
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



</script>

<div id="content" class="clear">
    <form class="form-horizontal">
        <div class="col-md-12 query" style="background: #3298d2">
            <div id="change"></div>
            <div class="form-group">
                <div class="col-xs-3">
                    <div class="input-group">
                        <input type="radio" id="dayreport_radio" name="query_radio" theme="simple" checked="true"/>
                        <span class="input-group-addon">日报表查询时间</span>

                        <div class="input-group date form_date">
                            <input class="form-control laydate-icon layer-date" id="dayReportDate"/>
                        </div>
                    </div>
                </div>
                <div class="col-xs-3">
                    <div class="input-group">
                        <input type="radio" id="monthreport_radio" name="query_radio" align="middle"/>
                        <span class="input-group-addon">月报表查询时间</span>

                        <div class="input-group date form_date">
                            <input class="form-control laydate-icon layer-date" id="monthReportDate"/>
                        </div>
                    </div>
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

        </div>
    </form>
    <p></p>

    <p></p>
    <!--绘制报表-->
    <div style="float:none" align="center">
        <div id="searchID">


        </div>
        <%--<div class="col-md-12 ">--%>
        <%--<div class="panel-body">--%>
        <%--<div id="powerCurve" style="height: 600px;">--%>
        <%--</div>--%>
        <%--</div>--%>
        <%--</div>--%>
    </div>
</body>
</html>
