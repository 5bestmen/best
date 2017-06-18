<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html>
<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>${sessionScope.APP_NAME}-历史数据</title>

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

        .divcss5-right{width:25%; height:100%;border:1px solid #ffa007;float:left}

    </style>
</head>

<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/dist/js/echarts/echarts.js"/>"></script>
<script src="<c:url value="/resources/js/generalChart.js"/>"></script>
<script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>
<script src="<c:url value="/resources/dist/laydate/laydate.js"/>"></script>
<script src="<c:url value="/resources/js/sweetalert.min.js" />"></script>

<script type="text/javascript">

    // 加载日节电量曲线
    function loadCurve(stationID, deviceID, dataType, searchDate) {
        $.ajax({
            type: "POST",
            url: "/api/multiCharts/historyQueryOfDay",
            data: {
                stationId: stationID,
                deviceId: deviceID,
                dataType: dataType,
                date: searchDate.format("yyyy-MM-dd")
            },

            success: function (result) {

                if (result == null || result.length <= 0)
                {
                    swal({
                        title: "数据库中未查询到数据!",
                        text: "数据库中没有存储该日该点的数据!",
                        confirmButtonText: "确定"
                    });
                    return;
                }

                if (result.length < 17) {
                    swal({
                        title: "查询失败,请检查配置！",
                        text: "查询失败,请检查配置！",
                        confirmButtonText: "确定"
                    });
                    return;
                }

                var topName;
                var a, b, c;
                //var dataA, dataB, dataC;
                var dataA = new Array();
                var dataB = new Array();
                var dataC = new Array();
                switch (dataType) {
                    case "electric":
                        topName = ['Ia','Ib','Ic'];
                        var i = 0;
                        dataA[i] = result[0][0];
                        for (var j = 1; j < result[0].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataA[i] = result[0][j];

                            }
                        }
                        i = 0;
                        dataB[i] = result[1][0];
                        for (var j = 1; j < result[1].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataB[i] = result[1][j];
                            }
                        }
                        i = 0;
                        dataC[i] = result[2][0];
                        for (var j = 1; j < result[2].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataC[i] = result[2][j];
                            }
                        }
                        a = "Ia";
                        b = "Ib";
                        c = "Ic";
                        break;
                    case "voltage":
                        topName = ['Ua','Ub','Uc'];

                        var i = 0;
                        dataA[i] = result[4][0];
                        for (var j = 1; j < result[4].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataA[i] = result[4][j];
                            }
                        }
                        i = 0;
                        dataB[i] = result[5][0];
                        for (var j = 1; j < result[5].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataB[i] = result[5][j];
                            }
                        }
                        i = 0;
                        dataC[i] = result[6][0];
                        for (var j = 1; j < result[6].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataC[i] = result[6][j];
                            }
                        }
                        a = "Ua";
                        b = "Ub";
                        c = "Uc";
                        break;
                    case "activeP":
                        topName = ['Pa','Pb','Pc'];

                        var i = 0;
                        dataA[i] = result[8][0];
                        for (var j = 1; j < result[8].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataA[i] = result[8][j];
                            }
                        }
                        i = 0;
                        dataB[i] = result[9][0];
                        for (var j = 1; j < result[9].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataB[i] = result[9][j];
                            }
                        }
                        i = 0;
                        dataC[i] = result[10][0];
                        for (var j = 1; j < result[10].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataC[i] = result[10][j];
                            }
                        }

                        a = "Pa";
                        b = "Pb";
                        c = "Pc";
                        break;
                    case "activeQ":
                        topName = ['Qa','Qb','Qc'];

                        var i = 0;
                        dataA[i] = result[11][0];
                        for (var j = 1; j < result[11].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataA[i] = result[11][j];
                            }
                        }
                        i = 0;
                        dataB[i] = result[12][0];
                        for (var j = 1; j < result[12].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataB[i] = result[12][j];
                            }
                        }
                        i = 0;
                        dataC[i] = result[13][0];
                        for (var j = 1; j < result[13].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataC[i] = result[13][j];
                            }
                        }

                        a = "Qa";
                        b = "Qb";
                        c = "Qc";
                        break;
                    case "activeF":
                        topName = ['CosA','CosB','CosC'];

                        var i = 0;
                        dataA[i] = result[14][0];
                        for (var j = 1; j < result[14].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataA[i] = result[14][j];
                            }
                        }
                        i = 0;
                        dataB[i] = result[15][0];
                        for (var j = 1; j < result[15].length; j++) {
                            if (j % 6 == 0) {
                                i++;
                                dataB[i] = result[15][j];
                            }
                        }
                        i = 0;
                        dataC[i] = result[16][0];
                        for (var j = 1; j < result[16].length; j++) {
                            if ((j+1) % 6 == 0) {
                                i++;
                                dataC[i] = result[16][j];
                            }
                        }

                        a = "CosA";
                        b = "CosB";
                        c = "CosC";
                        break;
                    default :
                        swal({
                            title: "数据类型选择有误!",
                            text: "数据类型选择有误!",
                            confirmButtonText: "确定"
                        });
                        return;
                }

                loadDataOfCurve(topName, a, b, c, dataA, dataB, dataC);
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

    function loadDataOfCurve(topName, a, b, c, dataA, dataB, dataC) {

        // 指定图表的配置项和数据
        var option = {
            tooltip : {
                trigger: 'axis'
            },
            legend: {
                data:topName,
                textStyle: {
                    fontSize: 16,
                    color: 'black'
                }
            },
            toolbox: {
                show : true,
                feature : {
                    mark : {show: true},
                    //dataView : {show: true, readOnly: false},
                    magicType : {show: true, type: ['line', 'bar', 'stack', 'tiled']},
                    restore : {show: true},
                    saveAsImage : {show: true}
                }
            },
            calculable : true,
            xAxis : [
                {
                    type : 'category',
                    boundaryGap : false,
                    data : ['0','','1','','2','','3','','4','','5','','6','','7','','8','','9','','10','','11','',
                        '12','','13','','14','','15','','16','','17','','18','','19','','20','','21','','22','','23',''],
                    axisLabel: {
                        show: true,
                        textStyle: {
                            color: '#FF7F00'
                        }
                    }
                }
            ],
            yAxis : [
                {
                    type : 'value',
                    axisLabel: {
                        show: true,
                        textStyle: {
                            color: '#FF7F00'
                        }
                    }
                }
            ],
            color: ['#FFB90F','#458B00','#FF0000'],
            series : [
                {
                    name:a,
                    type:'line',
                    data:dataA
                },
                {
                    name:b,
                    type:'line',
                    data:dataB
                },
                {
                    name:c,
                    type:'line',
                    data:dataC
                }
            ]
        };

        // 基于准备好的dom，初始化echarts实例
        var myChart = echarts.init(document.getElementById('curve'));

        // 使用刚指定的配置项和数据显示图表。
        myChart.setOption(option);
    }
</script>

<script>

    var dataType;
    function selectDataType(id){
        dataType = id;
    }

    function refresh(){
        var level = parent.getTreeLevel();
        if (level != 4) {
            swal({
                title: "请先选择设备再查询！",
                text: "请先选择设备再查询！",
                confirmButtonText: "确定"
            });
            return;
        }

        query();
    }

    $(function () {
        var initFlag = init();
        if (!initFlag) {
            return;
        }

        query();
    });

    function query() {

        timeInit();

        var hisSearchDate = {
            elem: '#historyDateID',
            format: 'YYYY-MM-DD',
            istime: true,
            istoday: true
        };

        laydate(hisSearchDate);

        $("#query").click(function () {

            var level = parent.getTreeLevel();
            if (level != 4) {
                 swal({
                 title: "请选择具体设备再查询！",
                 text: "请选择具体设备再查询！",
                 confirmButtonText: "确定"
                 });
                 return;
            }

            var a, b, c;
            switch (dataType) {
                case "electric":
                    a = "Ia";
                    b = "Ib";
                    c = "Ic";
                    break;
                case "voltage":
                    a = "Ua";
                    b = "Ub";
                    c = "Uc";
                    break;
                case "activeP":
                    a = "Pa";
                    b = "Pb";
                    c = "Pc";
                    break;
                case "activeQ":
                    a = "Qa";
                    b = "Qb";
                    c = "Qc";
                    break;
                case "activeF":
                    a = "CosA";
                    b = "CosB";
                    c = "CosC";
                    break;
                default :
                    swal({
                        title: "请选择数据类型!",
                        text: "请选择数据类型!",
                        confirmButtonText: "确定"
                    });
                    return;
            }

            // 时间合法性check
            var today = new Date();
            var date = $("#historyDateID").val();
            if (date.format("yyyy-MM-dd") >= today.format("yyyy-MM-dd")) {
                swal({
                    title: "查询日期必须在今天之前!",
                    text: "查询时间请重新选择!",
                    confirmButtonText: "确定"
                });
                return;
            }

            var stationID = parent.getTreeStationID();
            var stationName = parent.getTreeStationName();
            var deviceID = parent.getTreeDeviceID();
            var deviceName = parent.getTreeText();

            // 加载曲线
            loadCurve(stationID, deviceID, dataType, date);

            $.ajax({
                type: "POST",
                url: "/api/multiCharts/historyDataOfDay",
                data: {
                    stationId: stationID,
                    deviceId: deviceID,
                    stationName: stationName,
                    deviceName: deviceName,
                    dataType: dataType,
                    date: date
                },

                success: function (result) {

                    if (result.length <= 0)
                    {
                        swal({
                            title: "数据库中未查询到数据!",
                            text: "数据库中没有存储数据!",
                            confirmButtonText: "确定"
                        });
                        return;
                    }

                    var tableObj = $('#hisDataTable');
                    tableObj.empty();

                    var head = "<table class=\"hovertable\"; style=\"float:left;height:30%;width: 100%;\">";
                    head += "<tr>";
                    head += "<th></th><th style=\"text-align:center\">最大值</th><th style=\"text-align:center\">发生时间</th><th style=\"text-align:center\">最小值</th><th style=\"text-align:center\">发生时间</th>";
                    head += "</tr>";
                    head += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                    head += "<td style=\"text-align:center\">"+ a +"</td>";
                    head += "<td style=\"text-align:center\">"+result[0].maxValue+"</td><td style=\"text-align:center\">" + date + " " + result[0].maxTime+"</td><td style=\"text-align:center\">"+result[0].minValue+"</td><td style=\"text-align:center\">" + date + " " + result[0].minTime+"</td>";
                    head += "</tr>";
                    head += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                    head += "<td style=\"text-align:center\">"+ b +"</td>";
                    head += "<td style=\"text-align:center\">"+result[1].maxValue+"</td><td style=\"text-align:center\">" + date + " " + result[1].maxTime+"</td><td style=\"text-align:center\">"+result[1].minValue+"</td><td style=\"text-align:center\">" + date + " " + result[1].minTime+"</td>";
                    head += "</tr>";
                    head += "<tr onmouseover=\"this.style.backgroundColor='#ffff66';\" onmouseout=\"this.style.backgroundColor='#d4e3e5';\">";
                    head += "<td style=\"text-align:center\">"+ c +"</td>";
                    head += "<td style=\"text-align:center\">"+result[2].maxValue+"</td><td style=\"text-align:center\">" + date + " " + result[2].maxTime+"</td><td style=\"text-align:center\">"+result[2].minValue+"</td><td style=\"text-align:center\">" + date + " " + result[2].minTime+"</td>";
                    head += "</tr>";
                    head += "</table>";

                    tableObj.append(head);

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
    }

    function init() {
        var level = parent.getTreeLevel();
        if (level != 4) {

            swal({
                title: "请先选择设备！",
                text: "请先选择设备！",
                confirmButtonText: "确定"
            });

            return false;
        }

        return true;
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
        $('#historyDateID').val(mainData);
    }
</script>


<div class="row" style="margin:20px;">
    <div class="row" style="margin:0 auto;">

        <div class="col-xs-2"; style="float:left">
            <div class="input-group">
                <div class="input-group date form_date">
                    <input class="form-control laydate-icon layer-date" id="historyDateID"/>
                </div>
            </div>
        </div>

        <div style="float: left">
            <select onchange="selectDataType(this.options[selectedIndex].id)" name="dataType" class="form-control">
                <option id="head" style="background-color:blue;color:black" selected>数据类型</option>
                <option id="electric" style="background-color:orangered;color:black">电流</option>
                <option id="voltage" style="background-color:yellow;color:black">电压</option>
                <option id="activeP" style="background-color:orangered;color:black">有功功率</option>
                <option id="activeQ" style="background-color:yellow;color:black">无功功率</option>
                <option id="activeF" style="background-color:orangered;color:black">功率因数</option>
            </select>
        </div>

        <div style="float: left;margin:0 20px" class="btn-group" role="group">
            <button type="button" data-toggle="tooltip" data-placement="right" title=""
                    data-original-title="查询" class="btn btn-primary glyphicon glyphicon-search"
                    id="query">
            </button>
        </div>

    </div>

    <div style="margin:30px 0">
        <div class="col-md-6 measurement-container" style="width: 100%">
            <div class="panel panel-transparent">
                <div class="panel-body">
                    <div id="curve" style="height: 350px ; width: 100%;"></div>
                </div>

            </div>
        </div>
    </div>
</div>

<div id="hisDataTable" style="margin:40px 0">
    <table class="hovertable"; style="height:30%;width: 100%;">
        <tr>
            <th></th><th>最大值</th><th>发生时间</th><th>最小值</th><th>发生时间</th>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <td>-</td><td>-</td><td>-</td><td>-</td><td>-</td>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <td>-</td><td>-</td><td>-</td><td>-</td><td>-</td>
        </tr>
        <tr onmouseover="this.style.backgroundColor='#ffff66';" onmouseout="this.style.backgroundColor='#d4e3e5';">
            <td>-</td><td>-</td><td>-</td><td>-</td><td>-</td>
        </tr>
    </table>
</div>
</body>

</html>
