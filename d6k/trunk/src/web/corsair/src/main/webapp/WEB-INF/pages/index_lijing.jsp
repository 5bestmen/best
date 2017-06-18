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

    <title>${sessionScope.APP_NAME}-综合数据</title>

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

        .divcss5-right{width:25%; height:100%;float:right}



    </style>
</head>

<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
<script src="<c:url value="/resources/js/generalChart.js"/>"></script>
<script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>


<script src="<c:url value="/resources/dist/laydate/laydate.js"/>"></script>
<script src="<c:url value="/resources/js/sweetalert.min.js" />"></script>

<script>
    var dayOption = {
        tooltip: {
            trigger: 'axis'
        },
        legend: {
            padding: 5,
            textStyle: {
                color: 'white'
            },
            data: ['日节电量']
        },
        toolbox: {
            show: false,
            feature: {
                magicType: {show: true, type: ['line', 'bar']},
                restore: {show: true}
            }
        },
        xAxis: [
            {
                type: 'category',
                boundaryGap: true,
                axisLine: {onZero: false},
                axisLabel: {
                    textStyle: {
                        color: 'white'
                    }
                },
                data: []
            }
        ],
        yAxis: [
            {
                name: '日节电量(kWh)',
                nameTextStyle: {
                    color: 'white'
                },
                axisLabel: {
                    textStyle: {
                        color: 'white'
                    }
                },
                type: 'value'
            }
        ],
        series: [
            {
                name: '日节电量',
                type: 'bar',
                itemStyle: {
                    normal: {
                        color: '#32d2c9',
                        label: {show: true}
                    }
                },
                data: []
            }
        ],
        grid: {
            x: '10%',
            y: '10%',
            x2: '5%',
            y2: '10%'
        }
    };

    // 加载日节电量曲线
    function loadCurveOfDay(chooseDate, chooseName) {
        var level = parent.getTreeLevel();
        if (level != 1 && level != 2) {
            swal({
                title: "请选择省份或者地级市再查询！",
                text: "请选择省份或者地级市再查询！",
                confirmButtonText: "确定"
            });
            return;
        }

        var optionDayKwh = {
            ajax: true,
            URL: '/api/charts/stations/saveElecOfDay',
            robin: false,
            timeSpan: 60000, //日发电量1分钟更新一次
            getConfig: function () {
                return {level: level, name: chooseName, date: chooseDate};
            },
            formatNumber: true,
            EchartsOption: dayOption,
            chartContainer: 'dayKwh'
        };
        var dayKwhChart = $.fn.GeneralChart.init(optionDayKwh);
        dayKwhChart.load();
    }
//    $(function () {
//        var dayKwhChart = $.fn.GeneralChart.init(optionDayKwh);
//        dayKwhChart.load();
//    });
</script>

<script>
    var monthOption = {
        tooltip: {
            trigger: 'axis'
        },
        legend: {
            padding: 5,
            textStyle: {
                color: 'white'
            },
            data: ['月节电量']
        },
        toolbox: {
            show: false,
            feature: {
                magicType: {show: true, type: ['line', 'bar']},
                restore: {show: true}
            }
        },
        xAxis: [
            {
                type: 'category',
                boundaryGap: true,
                axisLine: {onZero: false},
                axisLabel: {
                    textStyle: {
                        color: 'white'
                    }
                },
                data: []
            }
        ],
        yAxis: [
            {
                name: '月节电量(kWh)',
                nameTextStyle: {
                    color: 'white'
                },
                axisLabel: {
                    textStyle: {
                        color: 'white'
                    }
                },
                type: 'value'
            }
        ],
        series: [
            {
                name: '月节电量',
                type: 'bar',
                itemStyle: {
                    normal: {
                        color: '#32d2c9',
                        label: {show: true}
                    }
                },
                data: []
            }
        ],
        grid: {
            x: '10%',
            y: '10%',
            x2: '5%',
            y2: '10%'
        }
    };

    // 加载月节电量曲线
    function loadCurveOfMonth(chooseDate, chooseName) {
        var level = parent.getTreeLevel();
        if (level != 1 && level != 2) {
            swal({
                title: "请选择省份或者地级市再查询！",
                text: "请选择省份或者地级市再查询！",
                confirmButtonText: "确定"
            });
            return;
        }

        var optionMonthKwh = {
            ajax: true,
            URL: '/api/charts/stations/saveElecOfMonth',
            robin: false,
            timeSpan: 60000, //日发电量1分钟更新一次
            getConfig: function () {
                return {level: level, name: chooseName, date: chooseDate};
            },
            formatNumber: true,
            EchartsOption: monthOption,
            chartContainer: 'monthKwh'
        };

        var monthKwhChart = $.fn.GeneralChart.init(optionMonthKwh);
        monthKwhChart.load();
    }

</script>

<script>
    var yearOption = {
        tooltip: {
            trigger: 'axis'
        },
        legend: {
            padding: 5,
            textStyle: {
                color: 'white'
            },
            data: ['年节电量']
        },
        toolbox: {
            show: false,
            feature: {
                magicType: {show: true, type: ['line', 'bar']},
                restore: {show: true}
            }
        },
        xAxis: [
            {
                type: 'category',
                boundaryGap: true,
                axisLine: {onZero: false},
                axisLabel: {
                    textStyle: {
                        color: 'white'
                    }
                },
                data: []
            }
        ],
        yAxis: [
            {
                name: '年节电量(kWh)',
                nameTextStyle: {
                    color: 'white'
                },
                axisLabel: {
                    textStyle: {
                        color: 'white'
                    }
                },
                type: 'value'
            }
        ],
        series: [
            {
                name: '年节电量',
                type: 'bar',
                itemStyle: {
                    normal: {
                        color: '#32d2c9',
                        label: {show: true}
                    }
                },
                data: []
            }
        ],
        grid: {
            x: '10%',
            y: '10%',
            x2: '5%',
            y2: '10%'
        }
    };

    // 加载年节电量曲线
    function loadCurveOfYear(chooseDate, chooseName) {
        var level = parent.getTreeLevel();
        if (level != 1 && level != 2) {
            swal({
                title: "请选择省份或者地级市再查询！",
                text: "请选择省份或者地级市再查询！",
                confirmButtonText: "确定"
            });
            return;
        }

        var optionYearKwh = {
            ajax: true,
            URL: '/api/charts/stations/saveElecOfYear',
            robin: false,
            timeSpan: 60000, //日发电量1分钟更新一次
            getConfig: function () {
                return {level: level, name: chooseName, date: chooseDate};
            },
            formatNumber: true,
            EchartsOption: yearOption,
            chartContainer: 'yearKwh'
        };

        var yearKwhChart = $.fn.GeneralChart.init(optionYearKwh);
        yearKwhChart.load();
    }

</script>

<script>
    $(function () {
        var meaObject = $.fn.GeneralMeasurement.init();
        meaObject.load();

        timeInit();

        var mainData = {
            elem: '#totalDateID',
            format: 'YYYY-MM-DD',
            istime: true,
            istoday: true
        };

        laydate(mainData);

        loadDeviceRunningInfo(true);

        $("#query").click(function () {

            var name = parent.getTreeText();
            var level = parent.getTreeLevel();

            if (level != 1 && level != 2) {
                swal({
                    title: "请选择省份或者地级市再查询！",
                    text: "请选择省份或者地级市再查询！",
                    confirmButtonText: "确定"
                });
                return;
            }

            // 时间合法性check
            var today = new Date();
            var date = $("#totalDateID").val();
            if (date.format("yyyy-MM-dd") >= today.format("yyyy-MM-dd")) {
                swal({
                    title: "查询日期必须在今天之前!",
                    text: "查询时间请重新选择!",
                    confirmButtonText: "确定"
                });
                return;
            }

            // 加载日节电量曲线
            loadCurveOfDay(date, name);

            // 加载月节电量曲线
            loadCurveOfMonth(date, name);

            // 加载年节电量曲线
            loadCurveOfYear(date, name);

            $.ajax({
                type : "POST",
                url: "/api/realtimeCharts/totalDataCalc",
                async : false,
                data : {
                    "name": name,
                    "type": level,
                    "date": date
                },
                success : function(result)
                {
                    var dayLen = result.dayCalcList.length;
                    if (dayLen == 0) {
                        swal({
                            title: "数据库中没有数据！",
                            text: "数据库中没有数据！",
                            confirmButtonText: "确定"
                        });
                        return;
                    }

                    if (dayLen < 12) {
                        swal({
                            title: "查询失败,请检查配置！",
                            text: "查询失败,请检查配置！",
                            confirmButtonText: "确定"
                        });
                        return;
                    }

                    var totalData = $('#totalData tbody');
                    totalData.empty();

                    var searchYear = date.substring(0, 4);
                    var searchMonth = date.substring(5, 7);
                    var searchDay = date.substring(8, 10);

                    var tr = "<tr>";
                    tr += "<th style=\"text-align:center; color:red;\">"+name+"</th><th style=\"text-align:center\">节电量</th><th style=\"text-align:center\">低电压率</th><th style=\"text-align:center\">调容次数</th><th style=\"text-align:center\">调压次数</th>";

                    tr += "</tr>";
                    tr += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                    tr += "<td style=\"text-align:center\">"+searchYear+"年</td>";
                    tr += "<td style=\"text-align:center\">" + result.dayCalcList[2].fValue + "</td>";
                    tr += "<td style=\"text-align:center\">" + result.dayCalcList[5].fValue + "</td>";
                    tr += "<td style=\"text-align:center\">" + result.dayCalcList[8].fValue + "</td>";
                    tr += "<td style=\"text-align:center\">" + result.dayCalcList[11].fValue + "</td>";

                    tr += "</tr>";
                    tr += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                    tr += "<td style=\"text-align:center\">"+searchMonth+"月</td>";
                    tr += "<td style=\"text-align:center\">"+result.dayCalcList[1].fValue+"</td>";
                    tr += "<td style=\"text-align:center\">"+result.dayCalcList[4].fValue+"</td>";
                    tr += "<td style=\"text-align:center\">"+result.dayCalcList[7].fValue+"</td>";
                    tr += "<td style=\"text-align:center\">"+result.dayCalcList[10].fValue+"</td>";

                    tr += "</tr>";
                    tr += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                    tr += "<td style=\"text-align:center\">"+searchDay+"日</td>";
                    tr += "<td style=\"text-align:center\">"+result.dayCalcList[0].fValue+"</td>";
                    tr += "<td style=\"text-align:center\">"+result.dayCalcList[3].fValue+"</td>";
                    tr += "<td style=\"text-align:center\">"+result.dayCalcList[6].fValue+"</td>";
                    tr += "<td style=\"text-align:center\">"+result.dayCalcList[9].fValue+"</td>";


                    tr += "</tr>";

                    totalData.append(tr);

                    var meaObject = $.fn.GeneralMeasurement.init();
                    meaObject.load();

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
        $('#totalDateID').val(mainData);
    }

    function refresh(){
        var level = parent.getTreeLevel();
        if (level != 1 && level != 2) {

            return;
        }

        loadDeviceRunningInfo(false)
    }



    function loadDeviceRunningInfo(check) {

        if (check) {
            var level = parent.getTreeLevel();
            if (level != 1 && level != 2) {
                swal({
                    title: "请选择省份或者地级市再查询！",
                    text: "请选择省份或者地级市再查询！",
                    confirmButtonText: "确定"
                });
                return;
            }
        }

        var name = parent.getTreeText();
        var level = parent.getTreeLevel();

        $.ajax({
            type : "POST",
            url: "/api/realtimeCharts/rdbDeviceRunningInfo",
            async : false,
            data : {
                "name": name,
                "type": level
            },
            success : function(result)
            {

                var tableObjInfo = $('#deviceRunningInfo');
                tableObjInfo.empty();


                var tr = "<div style=\"text-align: center\">";
                tr += "<span>终端数量:</span>";
                tr += "<span>" + result.nDeviceNumOfTotal + "</span>";
                tr += "</div>";
                tr += "<div style=\"text-align: center\">";
                tr += "<span>在线数:</span>";
                tr += "<span id=\"runningNum\" class=\"measurement\" data-PSRID=\"00521c0727v074\" data-measurementID=" + result.strDeviceNumOfRunning + " data-measurementType=\"3\"></span>";
                tr += "</div>";
                tr += "<div style=\"text-align: center\">";
                tr += "<span>离线数:</span>";
                tr += "<span class=\"measurement\" data-PSRID=\"00521c0727v074\" data-measurementID=" + result.strDeviceNumOfStoping + " data-measurementType=\"3\"></span>";
                tr += "</div>";
                tr += "<div style=\"text-align: center\">";
                tr += "<span>投入率:</span>";
                tr += "<span class=\"measurement\" data-PSRID=\"00521c0727v074\" data-measurementID=" + result.strPercent + " data-measurementType=\"3\"></span>";
                tr += "<span>%</span>";
                tr += "</div>";

                tableObjInfo.append(tr);

                var meaObject = $.fn.GeneralMeasurement.init();
                meaObject.load();
            }
        });
    }

    </script>

    <div class="row" style="margin:20px;">
        <div class="row" style="margin:0 auto;">
            <table id="totalData"; class="hovertable"; style="float:left;width: 40%;">
                <tr>
                    <th></th><th style="text-align:center">节电量</th><th style="text-align:center">低电压率</th><th style="text-align:center">调容次数</th><th style="text-align:center">调压次数</th>
                </tr>
                <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
                    <td style="text-align:center">年</td><td style="text-align:center">-</td><td style="text-align:center">-</td><td style="text-align:center">-</td><td style="text-align:center">-</td>
                </tr>
                <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
                    <td style="text-align:center">月</td><td style="text-align:center">-</td><td style="text-align:center">-</td><td style="text-align:center">-</td><td style="text-align:center">-</td>
                </tr>
                <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
                    <td style="text-align:center">日</td><td style="text-align:center">-</td><td style="text-align:center">-</td><td style="text-align:center">-</td><td style="text-align:center">-</td>
                </tr>
            </table>

            <div class="col-xs-2"; style="float:left;margin:40px 0">
                <div class="input-group">
                    <div class="input-group date form_date">
                        <input class="form-control laydate-icon layer-date" id="totalDateID"/>
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

            <div id="deviceRunningInfo" class="divcss5-right" >
                <div style="text-align: center">
                    <span>终端数量:</span>
                    <span class="measurement" data-calculate="" data-name="TotalForwardActivePower" data-templateIndex="0" data-PSRID="00521c0727v074" data-measurementID="" data-measurementType="3" data-magic=""></span>
                </div>
                <div style="text-align: center">
                    <span>在线数:</span>
                    <span class="measurement" data-calculate="" data-name="TotalForwardActivePower" data-templateIndex="0" data-PSRID="00521c0727v074" data-measurementID="" data-measurementType="3" data-magic=""></span>
                </div>
                <div style="text-align: center">
                    <span>离线数:</span>
                    <span class="measurement" data-calculate="" data-name="TotalForwardActivePower" data-templateIndex="0" data-PSRID="00521c0727v074" data-measurementID="" data-measurementType="3" data-magic=""></span>
                </div>
                <div style="text-align: center">
                    <span>投入率:</span>
                    <span class="measurement" data-calculate="" data-name="TotalForwardActivePower" data-templateIndex="0" data-PSRID="00521c0727v074" data-measurementID="" data-measurementType="3" data-magic=""></span>
                    <span>%</span>
                </div>
            </div>
        </div>

        <div style="margin:20px 0">
            <div class="col-md-6 measurement-container" style="width: 100%;">
                <div class="panel panel-transparent">
                    <div class="panel-body">
                        <div id="dayKwh" style="height: 250px ; width: 100%;"></div>
                    </div>

                </div>
            </div>

            <div class="col-md-6 measurement-container" style="width: 100%;">
                <div class="panel panel-transparent">
                    <div class="panel-body">
                        <div id="monthKwh" style="height: 250px; width: 100%;"></div>
                    </div>
                </div>
            </div>

            <div class="col-md-6 measurement-container" style="width: 100%;">
                <div class="panel panel-transparent">
                    <div class="panel-body">
                        <div id="yearKwh" style="height: 250px; width: 100%;"></div>
                    </div>
                </div>
            </div>
        </div>
</div>
</body>

</html>
