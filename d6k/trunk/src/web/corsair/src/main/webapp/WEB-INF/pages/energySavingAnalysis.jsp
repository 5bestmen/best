<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>

<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://corsair.com" prefix="corsair" %>
<%
    session.getAttributeNames();
%>

<!DOCTYPE html>
<html lang="zh_CN">

<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>${sessionScope.APP_NAME}-节能分析</title>

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/img/sun.png"/>" rel="shortcut icon"/>

    <link href="<c:url value="/resources/css/multiSelect/sol.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/alertify.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/multiSelect/bootstrap-multiselect.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sweetalert.min.css" />" rel="stylesheet">

    <style type="text/css">
        table.hovertable {
            font-family: verdana,arial,sans-serif;
            font-size:11px;
            color:#333333;
            border-width: 1px;
            border-color: #999999;
            border-collapse: collapse;
        }
        table.hovertable th {
            background-color:#c3dde0;
            border-width: 1px;
            padding: 8px;
            border-style: solid;
            border-color: #a9c6c9;
        }
        table.hovertable tr {
            background-color:#d4e3e5;
        }
        table.hovertable td {
            border-width: 1px;
            padding: 8px;
            border-style: solid;
            border-color: #a9c6c9;
        }

        .laydate-icon {
            background-image: url(/resources/img/Calendar.png);
        }

        .col-xs-2 {
            padding-left: 0px;
            width: 20%;
            padding-left: 20px;
        }

    </style>
</head>

<body>

<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>
<script src="<c:url value="/resources/js/sweetalert.min.js" />"></script>

<script src="<c:url value="/resources/js/generalChart.js"/>"></script>
<script src="<c:url value="/resources/dist/laydate/laydate.js"/>"></script>

<script>
    $(function () {
        var meaObject = $.fn.GeneralMeasurement.init();
        meaObject.load();

        timeInit();

        var mainData = {
            elem: '#searchOfDay',
            format: 'YYYY-MM-DD',
            istime: true,
            istoday: true
        };

        laydate(mainData);

        $("#query").click(function () {

            var name = parent.getTreeText();
            var level = parent.getTreeLevel();

            if (level != 4) {
                swal({
                    title: "请从左侧树形结构选择指定设备！",
                    text: "请从左侧树形结构选择指定设备！",
                    confirmButtonText: "确定"
                });
                return;
            }

            // 时间合法性check
            var today = new Date();
            var searchDate = $("#searchOfDay").val();
            if (searchDate.format("yyyy-MM-dd") >= today.format("yyyy-MM-dd")) {
                swal({
                    title: "查询日期必须在今天之前!",
                    text: "查询时间请重新选择!",
                    confirmButtonText: "确定"
                });
                return;
            }

            var stationID = parent.getTreeStationID();
            var deviceID = parent.getTreeDeviceID();

            $.ajax({
                type : "POST",
                url: "/api/realtimeCharts/energySavingAnalysis",
                async : false,
                data : {
                    "date": searchDate,
                    "stationId": stationID,
                    "deviceId": deviceID

                },
                success : function(result)
                {
                    if (result.dayCalcList == null) {
                        swal({
                            title: "数据库中没有数据！",
                            text: "数据库中没有数据！",
                            confirmButtonText: "确定"
                        });
                        return;
                    }

                    var dayLen = result.dayCalcList.length;
                    if (dayLen == 0) {
                        swal({
                            title: "数据库中没有数据！",
                            text: "数据库中没有数据！",
                            confirmButtonText: "确定"
                        });
                        return;
                    }

                    if (dayLen < 18) {
                        swal({
                            title: "查询失败,请检查配置！",
                            text: "查询失败,请检查配置！",
                            confirmButtonText: "确定"
                        });
                        return;
                    }

                    var dataOfDay = $('#energyOfDay');
                    dataOfDay.empty();

                    var trOfDay = "<thead>";
                    trOfDay += "<tr style='font-weight : bold' class='table-menu'>";
                    trOfDay += "<td colspan='17' style='text-align:center;'>调容变节能日统计</td>";
                    trOfDay += "</tr>";
                    trOfDay += "<td style='text-align:center;' colspan=\"2\">设备名称</td>";
                    trOfDay += "<td style='text-align:center;'>日运行时间(分)</td>";
                    trOfDay += "<td style='text-align:center;'>日节能量(kw.h)</td>";
                    trOfDay += "<tr>";
                    trOfDay += "<td style='text-align:center;' rowspan=\"2\">"+name+"</td>";
                    trOfDay += "<td style='text-align:center;'>大容量</td>";
                    trOfDay += "<td style='text-align:center;'>"+result.dayCalcList[3].value+"</td>";
                    trOfDay += "<td style='text-align:center;' rowspan=\"2\">"+result.dayCalcList[0].value * result.dayCalcList[0].code +"</td>";
                    trOfDay += "</tr>";
                    trOfDay += "<tr>";
                    trOfDay += "<td style='text-align:center;'>小容量</td>";
                    trOfDay += "<td style='text-align:center;'>"+result.dayCalcList[0].value+"</td>";
                    trOfDay += "</tr>";
                    trOfDay += "</thead>";

                    dataOfDay.append(trOfDay);

                    var dataOfMonth = $('#energyOfMonth');
                    dataOfMonth.empty();

                    var trOfMonth = "<thead>";
                    trOfMonth += "<tr style='font-weight : bold' class='table-menu'>";
                    trOfMonth += "<td colspan='17' style='text-align:center;'>调容变节能月统计</td>";
                    trOfMonth += "</tr>";
                    trOfMonth += "<td style='text-align:center;' colspan=\"2\">设备名称</td>";
                    trOfMonth += "<td style='text-align:center;'>月运行时间(分)</td>";
                    trOfMonth += "<td style='text-align:center;'>月节能量(kw.h)</td>";
                    trOfMonth += "<tr>";
                    trOfMonth += "<td style='text-align:center;' rowspan=\"2\">"+name+"</td>";
                    trOfMonth += "<td style='text-align:center;'>大容量</td>";
                    trOfMonth += "<td style='text-align:center;'>"+result.dayCalcList[4].value+"</td>";
                    trOfMonth += "<td style='text-align:center;' rowspan=\"2\">"+result.dayCalcList[1].value * result.dayCalcList[0].code +"</td>";
                    trOfMonth += "</tr>";
                    trOfMonth += "<tr>";
                    trOfMonth += "<td style='text-align:center;'>小容量</td>";
                    trOfMonth += "<td style='text-align:center;'>"+result.dayCalcList[1].value+"</td>";
                    trOfMonth += "</tr>";
                    trOfMonth += "</thead>";

                    dataOfMonth.append(trOfMonth);

                    var dataOfYear = $('#energyOfYear');
                    dataOfYear.empty();

                    var trOfYear = "<thead>";
                    trOfYear += "<tr style='font-weight : bold' class='table-menu'>";
                    trOfYear += "<td colspan='17' style='text-align:center;'>调容变节能年统计</td>";
                    trOfYear += "</tr>";
                    trOfYear += "<td style='text-align:center;' colspan=\"2\">设备名称</td>";
                    trOfYear += "<td style='text-align:center;'>年运行时间(分)</td>";
                    trOfYear += "<td style='text-align:center;'>年节能量(kw.h)</td>";
                    trOfYear += "<tr>";
                    trOfYear += "<td style='text-align:center;' rowspan=\"2\">"+name+"</td>";
                    trOfYear += "<td style='text-align:center;'>大容量</td>";
                    trOfYear += "<td style='text-align:center;'>"+result.dayCalcList[5].value+"</td>";
                    trOfYear += "<td style='text-align:center;' rowspan=\"2\">"+result.dayCalcList[2].value * result.dayCalcList[0].code +"</td>";
                    trOfYear += "</tr>";
                    trOfYear += "<tr>";
                    trOfYear += "<td style='text-align:center;'>小容量</td>";
                    trOfYear += "<td style='text-align:center;'>"+result.dayCalcList[2].value+"</td>";
                    trOfYear += "</tr>";
                    trOfYear += "</thead>";

                    dataOfYear.append(trOfYear);

                    var dataOfEnergy = $('#energyOfRunningTime');
                    dataOfEnergy.empty();

                    var trOfEnergy = "<thead>";
                    trOfEnergy += "<tr style='font-weight : bold' class='table-menu'>";
                    trOfEnergy += "<td colspan='17' style='text-align:center;'>调容变电压档位运行时间统计</td>";
                    trOfEnergy += "</tr>";
                    trOfEnergy += "<td style='text-align:center;' colspan=\"2\">档位信息</td>";
                    trOfEnergy += "<td style='text-align:center;'>日运行时间(分)</td>";
                    trOfEnergy += "<td style='text-align:center;'>月运行时间(分)</td>";
                    trOfEnergy += "<td style='text-align:center;'>年运行时间(分)</td>";
                    trOfEnergy += "<tr>";
                    trOfEnergy += "<td style='text-align:center;' rowspan=\"3\">"+name+"</td>";
                    trOfEnergy += "<td style='text-align:center;'>高档</td>";
                    trOfEnergy += "<td style='text-align:center;'>"+result.dayCalcList[15].value+"</td>";
                    trOfEnergy += "<td style='text-align:center;'>"+result.dayCalcList[16].value+"</td>";
                    trOfEnergy += "<td style='text-align:center;'>"+result.dayCalcList[17].value+"</td>";
                    trOfEnergy += "</tr>";
                    trOfEnergy += "<tr>";
                    trOfEnergy += "<td style='text-align:center;'>中档</td>";
                    trOfEnergy += "<td style='text-align:center;'>"+result.dayCalcList[12].value+"</td>";
                    trOfEnergy += "<td style='text-align:center;'>"+result.dayCalcList[13].value+"</td>";
                    trOfEnergy += "<td style='text-align:center;'>"+result.dayCalcList[14].value+"</td>";
                    trOfEnergy += "</tr>";
                    trOfEnergy += "<tr>";
                    trOfEnergy += "<td style='text-align:center;'>低档</td>";
                    trOfEnergy += "<td style='text-align:center;'>"+result.dayCalcList[9].value+"</td>";
                    trOfEnergy += "<td style='text-align:center;'>"+result.dayCalcList[10].value+"</td>";
                    trOfEnergy += "<td style='text-align:center;'>"+result.dayCalcList[11].value+"</td>";
                    trOfEnergy += "</tr>";
                    trOfEnergy += "</thead>";

                    dataOfEnergy.append(trOfEnergy);

                    // pie饼图数据
                    // 容量 Volume
                    // 加载饼状图
                    loadPieOfVolume(result.dayCalcList[0].value, result.dayCalcList[3].value);

                    loadPieOfU(result.dayCalcList[15].value, result.dayCalcList[12].value, result.dayCalcList[9].value);
                },
                error: function (errorMsg) {
                    swal({
                        title: "不好意思，图表请求数据失败啦!",
                        text: "不好意思，图表请求数据失败啦!",
                        confirmButtonText: "确定"
                    });
                }
            });
        });
    });
</script>

<script>
    function loadPieData() {
        $.ajax({
            type: "POST",
            url: "/api/realtimeCharts/energySavingPie",
            async: false,
            data: {
                "name": name
            },
            success: function (result) {
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
        var mainData;
        mainData = laydate.now(0, "YYYY-MM-DD");
        $('#searchOfDay').val(mainData);
    }

    function refresh(){
        var level = parent.getTreeLevel();
        if (level != 1 && level != 2) {

            return;
        }
    }

</script>

<script src="<c:url value="/resources/dist/js/echarts/echarts.js"/>"></script>
<script type="text/javascript">

    function loadPieOfVolume(small, big) {

        // 指定图表的配置项和数据
        var option = {
            title : {
                text: '日容量档位时间饼图',
                x:'center',
                textStyle: {
                    fontSize: 18,
                    fontWeight: 'bolder',
                    color: 'white'
                }
            },
            tooltip : {
                trigger: 'item',
                formatter: "{a} <br/>{b} : {c} ({d}%)"
            },
            legend: {
                orient : 'vertical',
                x : 'left',
                data:['大容量','小容量']
            },
            toolbox: {
                show : true,
                feature : {
                    mark : {show: true},
                    dataView : {show: true, readOnly: false},
                    magicType : {
                        show: true,
                        type: ['pie', 'funnel'],
                        option: {
                            funnel: {
                                x: '25%',
                                width: '50%',
                                funnelAlign: 'left',
                                max: 1548
                            }
                        }
                    },
                    restore : {show: true},
                    saveAsImage : {show: true}
                },
                textStyle: {
                    color: 'white'
                }
            },
            color:['rgb(252,157,154)','rgb(131,175,155)'],
            calculable : true,
            series : [
                {
                    name:'数据信息',
                    type:'pie',
                    radius : '55%',
                    center: ['50%', '60%'],
                    data:[
                        {value:big, name:'大容量'},
                        {value:small, name:'小容量'}
                    ],
                    itemStyle: {
                        normal: {
                            label: {
                                show: true,
                                formatter: '{b} : {c} \n ({d}%)'
                            },
                            labelLine: {
                                show: true
                            }
                        }
                    }
                }
            ]
        };

        // 基于准备好的dom，初始化echarts实例
        var myChart = echarts.init(document.getElementById('pieVolume'));

        // 使用刚指定的配置项和数据显示图表。
        myChart.setOption(option);
    }
</script>

<script type="text/javascript">

    function loadPieOfU(high, middle, low) {

        // 指定图表的配置项和数据
        var optionU = {
            title : {
                text: '日电压档位时间饼图',
                x:'center',
                textStyle: {
                    fontSize: 18,
                    fontWeight: 'bolder',
                    color: 'white'
                }
            },
            tooltip : {
                trigger: 'item',
                formatter: "{a} <br/>{b} : {c} ({d}%)"
            },
            legend: {
                orient : 'vertical',
                x : 'left',
                data:['高档','中档','低档']
            },
            toolbox: {
                show : true,
                feature : {
                    mark : {show: true},
                    dataView : {show: true, readOnly: false},
                    magicType : {
                        show: true,
                        type: ['pie', 'funnel'],
                        option: {
                            funnel: {
                                x: '25%',
                                width: '50%',
                                funnelAlign: 'left',
                                max: 1548
                            }
                        }
                    },
                    restore : {show: true},
                    saveAsImage : {show: true}
                },
                color:['green']
            },
            color: ['rgb(254,67,101)','rgb(252,157,154)','rgb(249,205,173)'],
            calculable : true,
            series : [
                {
                    name:'数据信息',
                    type:'pie',
                    radius : '55%',
                    center: ['50%', '60%'],
                    data:[
                        {value:high, name:'高档'},
                        {value:middle, name:'中档'},
                        {value:low, name:'低档'}
                    ],
                    itemStyle: {
                        normal: {
                            label: {
                                show: true,
                                formatter: '{b} : {c} \n ({d}%)'
                            },
                            labelLine: {
                                show: true
                            }
                        }
                    }
                }
            ]
        };

        // 基于准备好的dom，初始化echarts实例
        var myChart = echarts.init(document.getElementById('pieU'));

        // 使用刚指定的配置项和数据显示图表。
        myChart.setOption(optionU);
    }
</script>

<div class="row" style="margin:20px;">
    <div class="row" style="float:left; width: 70%;">
        <table id="energyOfYear"; class="hovertable"; style="width: 100%; height:22%">
            <thead>
            <tr style='font-weight : bold' class='table-menu'>
                <td colspan='17' style='text-align:center;'>调容变节能年统计</td>
            </tr>
            <td style='text-align:center;' colspan="2">设备名称</td>
            <td style='text-align:center;'>年运行时间(分)</td>
            <td style='text-align:center;'>年节能量(kw.h)</td>
            <tr>
                <td style='text-align:center;' rowspan="2">?</td>
                <td style='text-align:center;'>大容量</td>
                <td style='text-align:center;'>?</td>
                <td style='text-align:center;' rowspan="2">?</td>
            </tr>
            <tr>
                <td style='text-align:center;'>小容量</td>
                <td style='text-align:center;'>?</td>
            </tr>
            </thead>
        </table>

        <div style="margin:30px 0"></div>

        <table id="energyOfMonth"; class="hovertable"; style="width: 100%; height:22%">
            <thead>
            <tr style='font-weight : bold' class='table-menu'>
                <td colspan='17' style='text-align:center;'>调容变节能月统计</td>
            </tr>
            <td style='text-align:center;' colspan="2">设备名称</td>
            <td style='text-align:center;'>月运行时间(分)</td>
            <td style='text-align:center;'>月节能量(kw.h)</td>
            <tr>
                <td style='text-align:center;' rowspan="2">?</td>
                <td style='text-align:center;'>大容量</td>
                <td style='text-align:center;'>?</td>
                <td style='text-align:center;' rowspan="2">?</td>
            </tr>
            <tr>
                <td style='text-align:center;'>小容量</td>
                <td style='text-align:center;'>?</td>
            </tr>
            </thead>
        </table>

        <div style="margin:30px 0"></div>

        <table id="energyOfDay"; class="hovertable"; style="width: 100%; height:22%">
            <thead>
            <tr style='font-weight : bold' class='table-menu'>
                <td colspan='17' style='text-align:center;'>调容变节能日统计</td>
            </tr>
            <td style='text-align:center;' colspan="2">设备名称</td>
            <td style='text-align:center;'>日运行时间(分)</td>
            <td style='text-align:center;'>日节能量(kw.h)</td>
            <tr>
                <td style='text-align:center;' rowspan="2">?</td>
                <td style='text-align:center;'>大容量</td>
                <td style='text-align:center;'>?</td>
                <td style='text-align:center;' rowspan="2">?</td>
            </tr>
            <tr>
                <td style='text-align:center;'>小容量</td>
                <td style='text-align:center;'>?</td>
            </tr>
            </thead>
        </table>

        <div style="margin:30px 0"></div>

        <table id="energyOfRunningTime"; class="hovertable"; style="width: 100%; height:22%">
            <thead>
            <tr style='font-weight : bold' class='table-menu'>
                <td colspan='17' style='text-align:center;'>调容变电压档位运行时间统计</td>
            </tr>
            <td style='text-align:center;' colspan="2">档位信息</td>
            <td style='text-align:center;'>日运行时间(分)</td>
            <td style='text-align:center;'>月运行时间(分)</td>
            <td style='text-align:center;'>年运行时间(分)</td>
            <tr>
                <td style='text-align:center;' rowspan="3">?</td>
                <td style='text-align:center;'>高档</td>
                <td style='text-align:center;'>?</td>
                <td style='text-align:center;'>?</td>
                <td style='text-align:center;'>?</td>
            </tr>
            <tr>
                <td style='text-align:center;'>中档</td>
                <td style='text-align:center;'>?</td>
                <td style='text-align:center;'>?</td>
                <td style='text-align:center;'>?</td>
            </tr>
            <tr>
                <td style='text-align:center;'>低档</td>
                <td style='text-align:center;'>?</td>
                <td style='text-align:center;'>?</td>
                <td style='text-align:center;'>?</td>
            </tr>
            </thead>
        </table>

    </div>

    <div class="col-xs-2";  style="float:left;margin:40px 0">
        <div class="input-group">
            <div class="input-group date form_date">
                <input class="form-control laydate-icon layer-date" id="searchOfDay"/>
            </div>
        </div>
    </div>

    <div style="float:left;margin:40px 0">
        <div class="btn-group" role="group">
            <button type="button" data-toggle="tooltip" data-placement="right" title=""
                    data-original-title="查询" class="btn btn-primary glyphicon glyphicon-search"
                    id="query">
            </button>
        </div>
    </div>


    <div id="pieVolume" style="float:right; width: 30%;height:250px;"></div>

    <div style="margin:50px 0"></div>

    <div id="pieU" style="float:right; width: 30%;height:250px;"></div>
</div>

</body>

</html>
