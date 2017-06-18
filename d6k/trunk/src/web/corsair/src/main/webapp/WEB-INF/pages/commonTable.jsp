<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html>
<html lang="zh_CN">

<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>操作票</title>

    <link href="resources/css/bootstrap.min.css" rel="stylesheet">
    <link href="resources/css/metisMenu.min.css" rel="stylesheet">
    <link href="resources/css/sb-admin-2.css" rel="stylesheet">
    <link href="resources/css/font-awesome.min.css" rel="stylesheet" type="text/css">
    <link href="resources/css/style.css" rel="stylesheet" type="text/css">


    <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
    <script src="resources/dist/js/html5shiv.min.js"></script>
    <script src="resources/dist/js/respond.min.js"></script>
    <![endif]-->

    <script src="resources/dist/laydate/laydate.js"></script>
    <%--todo 页面自身需要的CSS begin--%>
    <link rel="stylesheet" type="text/css" href="resources/bower_components/dtGrid/css/jquery.dtGrid.min.css"/>
    <link rel="stylesheet" type="text/css" href="resources/bower_components/datePicker/skin/WdatePicker.css"/>
    <link rel="stylesheet" type="text/css" href="resources/bower_components/datePicker/skin/default/datepicker.css"/>
    <%--todo 页面自身需要的CSS end--%>
</head>

<body>
    <script src="resources/js/jquery.min.js"></script>
    <script src="resources/js/bootstrap.min.js"></script>
    <script src="resources/js/metisMenu.js"></script>
    <script src="resources/js/sb-admin-2.js"></script>
    <script type="text/javascript" src="resources/bower_components/dtGrid/js/jquery.dtGrid.min.js"></script>
    <script type="text/javascript" src="resources/bower_components/dtGrid/i18n/zh-cn.js"></script>
    <script type="text/javascript" src="resources/bower_components/datePicker/WdatePicker.js" defer="defer"></script>

<script>


</script>


<div id="wrapper">
    <jsp:include page="meun.jsp"/>


    <%--todo 工作空间，实际的内容将在这个位置添加--%>
    <div id="page-wrapper">
            <div class="row" >
                <div class="col-lg-12">
                    <div class="panel panel-primary">
                        <div class="panel-heading">
                            <i class="fa fa-pagelines fa-fw"></i>操作票
                        </div>
                        <ul class="nav nav-tabs">
                            <li><a class="tab-bg" data-toggle="tab" href="#ticket-tab-1">典型操作票1</a>
                            </li>
                            <li><a class="tab-bg" data-toggle="tab" href="#ticket-tab-2">典型操作票2</a>
                            </li>
                            <li><a class="tab-bg" data-toggle="tab" href="#ticket-tab-3">典型操作票3</a>
                            </li>
                            <li><a class="tab-bg" data-toggle="tab" href="#ticket-tab-4">自定义操作票</a>
                            </li>
                        </ul>
                        <div class="panel-body">

                            <div class="tab-content">
                                <div id="ticket-tab-1" class="tab-pane  active">
                                    <div id="common-ticket-1">
                                        <table border="1">
                                            <legend align="middle">典型操作票</legend>
                                            <tr>
                                                <td colspan="2">编号</td>
                                                <td colspan="13"><input class="form-control" id="series_num1" type="text" placeholder="编号"/></td>
                                            </tr>
                                            <tr>
                                                <td colspan="2">发令人</td>
                                                <td colspan="2"><input class="form-control" id="send_name1" type="text" placeholder="发令人姓名"/></td>
                                                <td colspan="2">受令人</td>
                                                <td colspan="2"><input class="form-control" id="recv_name1" type="text" placeholder="受令人姓名"/></td>
                                                <td colspan="2">发令时间</td>
                                                <td colspan="5"><input class='laydate-icon layer-date form-control' placeholder="发令时间" id="send_time1"></td>
                                            </tr>
                                            <tr>
                                                <td colspan="2">操作开始时间</td>
                                                <td colspan="6"><input class='col-md-4 laydate-icon layer-date form-control' placeholder="开始操作时间" id="begin_time1"></td>
                                                <td colspan="2">操作结束时间</td>
                                                <td colspan="5"><input class='col-md-4 laydate-icon layer-date form-control' placeholder="结束操作时间" id="end_time1"></td>
                                            </tr>
                                            <tr>
                                                <td colspan="2"><input class="col-md-2 form-control" id="guardian1" type="text" placeholder="监护人姓名"/></td>
                                                <td colspan="3">监护下操作</td>
                                                <td colspan="2"><input class="form-control" id="singleOperator1" type="text" placeholder="操作员姓名"/></td>
                                                <td colspan="3">单人操作</td>
                                                <td colspan="2"><input class="form-control" id="repairOperator1" type="text" placeholder="检修员姓名"/></td>
                                                <td colspan="3">检修人员操作</td>
                                            </tr>
                                            <tr>
                                                <td colspan="2">操作任务:</td>
                                                <td colspan="13">光伏电站启动运行</td>
                                            </tr>
                                            <tr>
                                                <td colspan="1">顺序</td>
                                                <td colspan="12">操作项目</td>
                                                <td colspan="2">完成情况</td>
                                            </tr>
                                            <tr>
                                                <td>1</td>
                                                <td colspan="12">检查并网柜光伏108线开关、箱变高低压开关、逆变器开关、汇流箱开关，处于断开状态，检查所有接线是否牢固。</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>2</td>
                                                <td colspan="12">确认现场没有检修人员，向调度汇报，返送电工作准备就绪，向调度报告</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>3</td>
                                                <td colspan="12">断开光伏箱变高压侧接地开关，闭合箱变高压侧开关，（第一次，使箱变带电２４小时）</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>4</td>
                                                <td colspan="12">断开112光伏线路接地开关，闭合光伏112线路开关，对10KV光伏112线路进行冲击，确认10KV光伏112线路冲击正常调试人员检查母线ＴＶ二次电压幅值及相序正确，操作完毕，汇报调度。</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>5</td>
                                                <td colspan="12">确认正常后，闭合光伏箱变11211低压断路器和光伏箱变11212低压断路器。</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>6</td>
                                                <td colspan="12">检查汇流箱，正负极接线是否正确、保险是否安装；</td>
                                                <td colspan="2"></td>
                                            </tr>

                                            <tr>
                                                <td>7</td>
                                                <td colspan="12">确认无误后，闭合汇流箱开关；</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>8</td>
                                                <td colspan="12">确认无误后，用万用表直流电压档测每台汇流箱出线正负极是否与逆变器直流侧接线柱正负极一致。</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>9</td>
                                                <td colspan="12">确认无误后，闭合＃１逆变器直流侧开关和＃２逆变器直流侧开关；</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>10</td>
                                                <td colspan="12">确认无误后，闭合＃１逆变器交流侧开关和＃２逆变器交流侧开关； </td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>11</td>
                                                <td colspan="12">启动＃１逆变器并网开关和＃2逆变器并网开关，并网成功。</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>备注:</td>
                                                <td colspan="14"><input class="form-control" id="comment1" type="text" placeholder="备注"/></td>
                                            </tr>
                                            <tr>
                                                <td colspan="2">操作人</td>
                                                <td colspan="3"><input class="form-control" id="operateName1" type="text" placeholder="操作人签名"/></td>
                                                <td colspan="2">监护人</td>
                                                <td colspan="3"><input class="form-control" id="guardianName1" type="text" placeholder="监护人签名"/></td>
                                                <td colspan="2">值班负责人（值长）:</td>
                                                <td colspan="3"><input class="form-control" id="DutyName1" type="text" placeholder="值长签名"/></td>
                                            </tr>
                                        </table>
                                        <button class="btn btn-primary" onclick="submitTicket1()">生成操作票</button>
                                        <script>

                                        var send_time_data1 = {
                                        elem: '#send_time1',
                                        format: 'YYYY-MM-DD hh:mm:ss',
                                        max: '2099-06-16',
                                        istime: true,
                                        istoday: true,

                                        }
                                        var begin_time_data1 = {
                                        elem: '#begin_time1',
                                        format: 'YYYY-MM-DD hh:mm:ss',
                                        max: '2099-06-16',
                                        istime: true,
                                        istoday: true,

                                        }
                                        var end_time_data1 = {
                                        elem: '#end_time1',
                                        format: 'YYYY-MM-DD hh:mm:ss',
                                        max: '2099-06-16',
                                        istime: true,
                                        istoday: true,
                                        }
                                        laydate(send_time_data1);
                                        laydate(begin_time_data1);
                                        laydate(end_time_data1);
                                        function submitTicket1(){
                                   //     var ticketTitle = $("#ticket_title1").val();
                                        var series_num = $("#series_num1").val();
                                        var sendName = $("#send_name1").val();
                                        var recvName = $("#recv_name1").val();
                                        var send_time_value = $("#send_time1").val();
                                        var begin_time_value = $("#begin_time1").val();
                                        var end_time_value = $("#end_time1").val();
                                        var guardianName = $("#guardian1").val();
                                        var singleOperatorName = $("#singleOperator1").val();
                                        var repairOperatorName = $("#repairOperator1").val();
                                        var comment = $("#comment1").val();
                                        var operatorName = $("#operateName1").val();
                                        var guardiansignName = $("#guardianName1").val();
                                        var DutyName = $("#DutyName1").val();



                                        $.post('/operateTicketServlet',{ticketType:'commonTicket1',seriesNum:series_num,sendName:sendName,recvName:recvName,sendTime:send_time_value,
                                        beginTime:begin_time_value,endTime:end_time_value,guardianName:guardianName,singleOperatorName:singleOperatorName,
                                        repairOperatorName:repairOperatorName,comment:comment,operatorSign:operatorName,guardianSign:guardiansignName,dutyName:DutyName},function (response) {
                                        var tesx = response;
                                        },'application/pdf');
                                            setTimeout(function(){window.open("http://localhost:8080/tickets/commonTicket1.pdf","_self");}, 2000);

                                        }

                                        </script>
                                    </div>
                                </div>

                                <div id="ticket-tab-2" class="tab-pane">
                                    <div id="common-ticket-2">
                                            <table border="1"  width="100%">
                                                <legend align="middle">典型操作票</legend>
                                                <tr>
                                                    <td colspan="2">编号</td>
                                                    <td colspan="13"><input class="form-control" id="series_num2" type="text" placeholder="编号"/></td>
                                                </tr>
                                                <tr >
                                                    <td colspan="2">发令人</td>
                                                    <td colspan="2"><input class="form-control" id="send_name2" type="text" placeholder="发令人姓名"/></td>
                                                    <td colspan="2">受令人</td>
                                                    <td colspan="2"><input class="form-control" id="recv_name2" type="text" placeholder="受令人姓名"/></td>
                                                    <td colspan="2">发令时间</td>
                                                    <td colspan="5"><input class='laydate-icon layer-date form-control' placeholder="发令时间" id="send_time2"></td>
                                                </tr>
                                                <tr>
                                                    <td colspan="2">操作开始时间</td>
                                                    <td colspan="6"><input class='col-md-4 laydate-icon layer-date form-control' placeholder="开始操作时间" id="begin_time2"></td>
                                                    <td colspan="2">操作结束时间</td>
                                                    <td colspan="5"><input class='col-md-4 laydate-icon layer-date form-control' placeholder="结束操作时间" id="end_time2"></td>
                                                </tr>
                                                <tr>
                                                    <td colspan="2"><input class="col-md-2 form-control" id="guardian2" type="text" placeholder="监护人姓名"/></td>
                                                    <td colspan="3">监护下操作</td>
                                                    <td colspan="2"><input class="form-control" id="singleOperator2" type="text" placeholder="操作员姓名"/></td>
                                                    <td colspan="3">单人操作</td>
                                                    <td colspan="2"><input class="form-control" id="repairOperator2" type="text" placeholder="检修员姓名"/></td>
                                                    <td colspan="3">检修人员操作</td>
                                                </tr>
                                                <tr>
                                                    <td colspan="2">操作任务:</td>
                                                    <td colspan="13">光伏升压箱变运行转检修</td>
                                                </tr>
                                                <tr>
                                                    <td colspan="1">顺序</td>
                                                    <td colspan="12">操作项目</td>
                                                    <td colspan="2">完成情况</td>
                                                </tr>
                                                <tr>
                                                    <td>1</td>
                                                    <td colspan="12">接值长令</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>2</td>
                                                    <td colspan="12">关闭光伏发电系统所有直流断路器</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>3</td>
                                                    <td colspan="12">将发电系统逆变器A交流侧旋钮由ON旋到OFF状态</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>4</td>
                                                    <td colspan="12">将发电系统逆变器B交流侧旋钮由ON旋到OFF状态</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>5</td>
                                                    <td colspan="12">断开11211低压断路器</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>6</td>
                                                    <td colspan="12">检查11211低压断路器是否处于断开状态，并悬挂“线路检修，禁止合闸”标识牌</td>
                                                    <td colspan="2"></td>
                                                </tr>

                                                <tr>
                                                    <td>7</td>
                                                    <td colspan="12">断开11212低压断路器</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>8</td>
                                                    <td colspan="12">检查11212低压断路器是否处于断开状态，并悬挂“线路检修，禁止合闸”标识牌</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>9</td>
                                                    <td colspan="12">报告调度，申请检修变压器，断开光伏112线开关</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>10</td>
                                                    <td colspan="12">执行调度命令，断开光伏112线开关</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>11</td>
                                                    <td colspan="12">旋转光伏112开关至分闸位置</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>12</td>
                                                    <td colspan="12">观察光伏112线小车上的112开关断开指示是否处于断开状态</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>13</td>
                                                    <td colspan="12">摇出112小车，并且确认检查接地开关在合闸位置</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>14</td>
                                                    <td colspan="12">检查光伏箱变高压侧负荷开关和接地开关，并且断开高压侧负荷开关；</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>15</td>
                                                    <td colspan="12">确认光伏箱变高压侧负荷开关断开后，闭合高压侧接地开关。</td>
                                                    <td colspan="2"></td>
                                                </tr>
                                                <tr>
                                                    <td>备注:</td>
                                                    <td colspan="14"><input class="form-control" id="comment2" type="text" placeholder="备注"/></td>
                                                </tr>
                                                <tr>
                                                    <td colspan="2">操作人</td>
                                                    <td colspan="3"><input class="form-control" id="operateName2" type="text" placeholder="操作人签名"/></td>
                                                    <td colspan="2">监护人</td>
                                                    <td colspan="3"><input class="form-control" id="guardianName2" type="text" placeholder="监护人签名"/></td>
                                                    <td colspan="2">值班负责人（值长）:</td>
                                                    <td colspan="3"><input class="form-control" id="DutyName2" type="text" placeholder="值长签名"/></td>
                                                </tr>
                                            </table>
                                            <button class="btn btn-primary" onclick="submitTicket2()">生成操作票</button>
                                            <script>
                                                var send_time_data2 = {
                                                    elem: '#send_time2',
                                                    format: 'YYYY-MM-DD hh:mm:ss',
                                                    max: '2099-06-16',
                                                    istime: true,
                                                    istoday: true,

                                                }
                                                var begin_time_data2 = {
                                                    elem: '#begin_time2',
                                                    format: 'YYYY-MM-DD hh:mm:ss',
                                                    max: '2099-06-16',
                                                    istime: true,
                                                    istoday: true,

                                                }
                                                var end_time_data2 = {
                                                    elem: '#end_time2',
                                                    format: 'YYYY-MM-DD hh:mm:ss',
                                                    max: '2099-06-16',
                                                    istime: true,
                                                    istoday: true,
                                                }
                                                laydate(send_time_data2);
                                                laydate(begin_time_data2);
                                                laydate(end_time_data2);
                                                function submitTicket2(){
                                                    //     var ticketTitle = $("#ticket_title2").val();
                                                    var series_num = $("#series_num2").val();
                                                    var sendName = $("#send_name2").val();
                                                    var recvName = $("#recv_name2").val();
                                                    var send_time_value = $("#send_time2").val();
                                                    var begin_time_value = $("#begin_time2").val();
                                                    var end_time_value = $("#end_time2").val();
                                                    var guardianName = $("#guardian2").val();
                                                    var singleOperatorName = $("#singleOperator2").val();
                                                    var repairOperatorName = $("#repairOperator2").val();
                                                    var comment = $("#comment2").val();
                                                    var operatorName = $("#operateName2").val();
                                                    var guardiansignName = $("#guardianName2").val();
                                                    var DutyName = $("#DutyName2").val();

                                                    $.post('/operateTicketServlet',{ticketType:'commonTicket2',seriesNum:series_num,sendName:sendName,recvName:recvName,sendTime:send_time_value,
                                                        beginTime:begin_time_value,endTime:end_time_value,guardianName:guardianName,singleOperatorName:singleOperatorName,
                                                        repairOperatorName:repairOperatorName,comment:comment,operatorSign:operatorName,guardianSign:guardiansignName,dutyName:DutyName},function (response) {
                                                        var tesx = response;
                                                    },'application/pdf');
                                                    setTimeout(function(){window.open("http://localhost:8080/tickets/commonTicket2.pdf","_self");}, 2000);

                                                }

                                            </script>
                                    </div>
                                </div>
                                <div id="ticket-tab-3" class="tab-pane">
                                    <div id="common-ticket-3">
                                        <table border="1"  width="100%">
                                            <legend align="middle">典型操作票</legend>
                                            <tr>
                                                <td colspan="2">编号</td>
                                                <td colspan="13"><input class="form-control" id="series_num3" type="text" placeholder="编号"/></td>
                                            </tr>
                                            <tr >
                                                <td colspan="2">发令人</td>
                                                <td colspan="2"><input class="form-control" id="send_name3" type="text" placeholder="发令人姓名"/></td>
                                                <td colspan="2">受令人</td>
                                                <td colspan="2"><input class="form-control" id="recv_name3" type="text" placeholder="受令人姓名"/></td>
                                                <td colspan="2">发令时间</td>
                                                <td colspan="5"><input class='laydate-icon layer-date form-control' placeholder="发令时间" id="send_time3"></td>
                                            </tr>
                                            <tr>
                                                <td colspan="2">操作开始时间</td>
                                                <td colspan="6"><input class='col-md-4 laydate-icon layer-date form-control' placeholder="开始操作时间" id="begin_time3"></td>
                                                <td colspan="2">操作结束时间</td>
                                                <td colspan="5"><input class='col-md-4 laydate-icon layer-date form-control' placeholder="结束操作时间" id="end_time3"></td>
                                            </tr>
                                            <tr>
                                                <td colspan="2"><input class="col-md-2 form-control" id="guardian3" type="text" placeholder="监护人姓名"/></td>
                                                <td colspan="3">监护下操作</td>
                                                <td colspan="2"><input class="form-control" id="singleOperator3" type="text" placeholder="操作员姓名"/></td>
                                                <td colspan="3">单人操作</td>
                                                <td colspan="2"><input class="form-control" id="repairOperator3" type="text" placeholder="检修员姓名"/></td>
                                                <td colspan="3">检修人员操作</td>
                                            </tr>
                                            <tr>
                                                <td colspan="2">操作任务:</td>
                                                <td colspan="13">光伏升压变由检修转运行</td>
                                            <tr>
                                                <td colspan="1">顺序</td>
                                                <td colspan="12">操作项目</td>
                                                <td colspan="2">完成情况</td>
                                            </tr>
                                            <tr>
                                                <td>1</td>
                                                <td colspan="12">接值长令</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>2</td>
                                                <td colspan="12">检查高压柜、升压变、逆变器、汇流箱断路器在断路状态</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>3</td>
                                                <td colspan="12">报告调度，申请112线路由检修转运行</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>4</td>
                                                <td colspan="12">检查光伏箱变高压侧负荷开关和接地开关，断开高压侧接地开关；</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>5</td>
                                                <td colspan="12">确认光伏箱变高压侧高压侧接地开关断开后，闭合光伏箱变高压侧负荷开关。</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>6</td>
                                                <td colspan="12">报告调度，申请送电</td>
                                                <td colspan="2"></td>
                                            </tr>

                                            <tr>
                                                <td>7</td>
                                                <td colspan="12">摇进112小车，并且确认检查接地开关在分闸位置</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>8</td>
                                                <td colspan="12">旋转光伏112开关至闭合位置</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>9</td>
                                                <td colspan="12">观察光伏112线小车上的112开关断开指示是否处于闭合状态</td>
                                                <td colspan="2"></td>
                                            </tr>
                                            <tr>
                                                <td>备注:</td>
                                                <td colspan="14"><input class="form-control" id="comment3" type="text" placeholder="备注"/></td>
                                            </tr>
                                            <tr>
                                                <td colspan="2">操作人</td>
                                                <td colspan="3"><input class="form-control" id="operateName3" type="text" placeholder="操作人签名"/></td>
                                                <td colspan="2">监护人</td>
                                                <td colspan="3"><input class="form-control" id="guardianName3" type="text" placeholder="监护人签名"/></td>
                                                <td colspan="2">值班负责人（值长）:</td>
                                                <td colspan="3"><input class="form-control" id="DutyName3" type="text" placeholder="值长签名"/></td>
                                            </tr>
                                        </table>
                                        <button class="btn btn-primary" onclick="submitTicket3()">生成操作票</button>
                                        <script>
                                            var send_time_data3 = {
                                                elem: '#send_time3',
                                                format: 'YYYY-MM-DD hh:mm:ss',
                                                max: '3099-06-16',
                                                istime: true,
                                                istoday: true,

                                            }
                                            var begin_time_data3 = {
                                                elem: '#begin_time3',
                                                format: 'YYYY-MM-DD hh:mm:ss',
                                                max: '2099-06-16',
                                                istime: true,
                                                istoday: true,

                                            }
                                            var end_time_data3 = {
                                                elem: '#end_time3',
                                                format: 'YYYY-MM-DD hh:mm:ss',
                                                max: '2099-06-16',
                                                istime: true,
                                                istoday: true,
                                            }
                                            laydate(send_time_data3);
                                            laydate(begin_time_data3);
                                            laydate(end_time_data3);
                                            function submitTicket3(){
                                                //     var ticketTitle = $("#ticket_title3").val();
                                                var series_num = $("#series_num3").val();
                                                var sendName = $("#send_name3").val();
                                                var recvName = $("#recv_name3").val();
                                                var send_time_value = $("#send_time3").val();
                                                var begin_time_value = $("#begin_time3").val();
                                                var end_time_value = $("#end_time3").val();
                                                var guardianName = $("#guardian3").val();
                                                var singleOperatorName = $("#singleOperator3").val();
                                                var repairOperatorName = $("#repairOperator3").val();
                                                var comment = $("#comment3").val();
                                                var operatorName = $("#operateName3").val();
                                                var guardiansignName = $("#guardianName3").val();
                                                var DutyName = $("#DutyName3").val();


                                                $.post('/operateTicketServlet',{ticketType:'commonTicket3',seriesNum:series_num,sendName:sendName,recvName:recvName,sendTime:send_time_value,
                                                    beginTime:begin_time_value,endTime:end_time_value,guardianName:guardianName,singleOperatorName:singleOperatorName,
                                                    repairOperatorName:repairOperatorName,comment:comment,operatorSign:operatorName,guardianSign:guardiansignName,dutyName:DutyName},function (response) {
                                                    var tesx = response;
                                                },'application/pdf');
                                                setTimeout(function(){window.open("http://localhost:8080/tickets/commonTicket3.pdf","_self");}, 2000);
                                            }

                                        </script>
                                        </div>
                                    </div>
                                <div id="ticket-tab-4" class="tab-pane">
                                    <div id="common-ticket-4">
                                        <table border="1"  width="100%" id="ticket">
                                            <legend align="middle"><input class="form-control" id="ticket_title" type="text" placeholder="操作票名称"/></legend>
                                            <tr>
                                                <td colspan="2">编号</td>
                                                <td colspan="13"><input class="form-control" id="series_num_diy" type="text" placeholder="编号"/></td>
                                            </tr>
                                            <tr >
                                                <td colspan="2">发令人</td>
                                                <td colspan="2"><input class="form-control" id="send_name_diy" type="text" placeholder="发令人姓名"/></td>
                                                <td colspan="2">受令人</td>
                                                <td colspan="2"><input class="form-control" id="recv_name_diy" type="text" placeholder="受令人姓名"/></td>
                                                <td colspan="2">发令时间</td>
                                                <td colspan="5"><input class='laydate-icon layer-date form-control' placeholder="发令时间" id="send_time_diy"></td>
                                            </tr>
                                            <tr>
                                                <td colspan="2">操作开始时间</td>
                                                <td colspan="6"><input class='col-md-4 laydate-icon layer-date form-control' placeholder="开始操作时间" id="begin_time_diy"></td>
                                                <td colspan="2">操作结束时间</td>
                                                <td colspan="5"><input class='col-md-4 laydate-icon layer-date form-control' placeholder="结束操作时间" id="end_time_diy"></td>
                                            </tr>
                                            <tr>
                                                <td colspan="2"><input class="col-md-2 form-control" id="guardian_diy" type="text" placeholder="监护人姓名"/></td>
                                                <td colspan="3">监护下操作</td>
                                                <td colspan="2"><input class="form-control" id="singleOperator_diy" type="text" placeholder="操作员姓名"/></td>
                                                <td colspan="3">单人操作</td>
                                                <td colspan="2"><input class="form-control" id="repairOperator_diy" type="text" placeholder="检修员姓名"/></td>
                                                <td colspan="3">检修人员操作</td>
                                            </tr>
                                            <tr rowspan="2">
                                                <td colspan="2">操作任务:</td>
                                                <td colspan="13"><textarea class="form-control" id="operate_task" rows="2" cols="200" placeholder="任务内容"></textarea></td>
                                            <tr>
                                                <td colspan="1">顺序</td>
                                                <td colspan="12">操作项目</td>
                                                <td colspan="1">完成情况</td>
                                                <td colspan="1"><button type="button" id="addItem" class="btn btn-primary">增加</button></td>
                                            </tr>
                                            <tr>
                                                <td><input class="form-control" name="seq" type="text" placeholder="序号"/></td>
                                                <td colspan="12"><textarea  class="form-control" name="operationColumn" rows="1" cols="200" type="text" placeholder="操作项目"></textarea></td>
                                                <td colspan="1"></td>
                                                <td colspan="1"></td>
                                            </tr>
                                        </table>
                                        <table border="1" width="100%">
                                            <tr>
                                                <td>备注:</td>
                                                <td colspan="14"><input class="form-control" id="comment_diy" type="text" placeholder="备注"/></td>
                                            </tr>
                                            <tr>
                                                <td colspan="2">操作人</td>
                                                <td colspan="3"><input class="form-control" id="operateName_diy" type="text" placeholder="操作人签名"/></td>
                                                <td colspan="2">监护人</td>
                                                <td colspan="3"><input class="form-control" id="guardianName_diy" type="text" placeholder="监护人签名"/></td>
                                                <td colspan="2">值班负责人（值长）:</td>
                                                <td colspan="3"><input class="form-control" id="DutyName_diy" type="text" placeholder="值长签名"/></td>
                                            </tr>
                                        </table>
                                        <button class="btn btn-primary" onclick="submitTicket_diy()">生成操作票</button>
                                            <script>

                                                $(function() {
                                                    $('#addItem').click(function () {
                                                        var i = 2;
                                                        if (i < 14) {
                                                            $('#ticket').append('<tr>'
                                                                    + '<td><input class="form-control" name="seq" type="text" placeholder="序号"/></td>'
                                                                    + '<td colspan="12"><textarea  class="form-control" name="operationColumn" rows="1" cols="200" type="text" placeholder="操作项目"></textarea></td>'
                                                                    + '<td colspan="1"></td>'
                                                                    + '<td colspan="1"><button type="button"  class="del-item btn btn-danger">删除</button></td>'
                                                                    + '</tr>');
                                                            i++;

                                                        } else {
                                                            alert("最多加14个");
                                                        }
                                                        $('.del-item').on('click', function () {
                                                            $(this).parent().parent().remove();
                                                            i--;
                                                        });
                                                    });

                                                });


                                                var send_time_data_diy = {
                                                    elem: '#send_time_diy',
                                                    format: 'YYYY-MM-DD hh:mm:ss',
                                                    max: '2099-06-16',
                                                    istime: true,
                                                    istoday: true,
                                                }
                                                var begin_time_data_diy = {
                                                    elem: '#begin_time_diy',
                                                    format: 'YYYY-MM-DD hh:mm:ss',
                                                    max: '2099-06-16',
                                                    istime: true,
                                                    istoday: true,

                                                }
                                                var end_time_data_diy = {
                                                    elem: '#end_time_diy',
                                                    format: 'YYYY-MM-DD hh:mm:ss',
                                                    max: '2099-06-16',
                                                    istime: true,
                                                    istoday: true,

                                                }
                                                laydate(send_time_data_diy);
                                                laydate(begin_time_data_diy);
                                                laydate(end_time_data_diy);
                                                function submitTicket_diy(){
                                                    var ticketTitle = $("#ticket_title").val();
                                                    var series_num = $("#series_num_diy").val();
                                                    var sendName = $("#send_name_diy").val();
                                                    var recvName = $("#recv_name_diy").val();
                                                    var send_time_value = $("#send_time_diy").val();
                                                    var begin_time_value = $("#begin_time_diy").val();
                                                    var end_time_value = $("#end_time_diy").val();
                                                    var guardianName = $("#guardian_diy").val();
                                                    var singleOperatorName = $("#singleOperator_diy").val();
                                                    var repairOperatorName = $("#repairOperator_diy").val();
                                                    var operateTask = $("#operate_task").val();
                                                    var comment = $("#comment_diy").val();
                                                    var operatorName = $("#operateName_diy").val();
                                                    var guardiansignName = $("#guardianName_diy").val();
                                                    var DutyName = $("#DutyName_diy").val();
                                                    var seq = new Array();
                                                    var operateColumn = new Array();

                                                    $("input[name='seq']").each(function () {
                                                        seq.push($(this).val());
                                                    });

                                                    $("textarea[name='operationColumn']").each(function () {
                                                        operateColumn.push($(this).val());
                                                    });



                                                    $.post('/operateTicketServlet',{ticketType:'diyTicket',ticketTitle:ticketTitle,seriesNum:series_num,sendName:sendName,recvName:recvName,sendTime:send_time_value,
                                                        beginTime:begin_time_value,endTime:end_time_value,guardianName:guardianName,singleOperatorName:singleOperatorName,
                                                        repairOperatorName:repairOperatorName,operateTask:operateTask,sequence:seq,operateColumn:operateColumn,comment:comment,operatorSign:operatorName,guardianSign:guardiansignName,dutyName:DutyName},function (response) {
                                                        var tesx = response;
                                                    },'application/pdf');

                                                    setTimeout(function(){window.open("http://localhost:8080/tickets/diyTicket.pdf","_self");}, 2000);
                                                }

                                            </script>
                                        </div>
                                    </div>
                                </div>
                              </div>

                            </div>
                        </div>
                    </div>
                </div>
            </div>
</div>

</body>

</html>
