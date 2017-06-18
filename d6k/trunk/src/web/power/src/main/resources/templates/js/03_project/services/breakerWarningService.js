/**
 * Created by lojoso on 2017/1/11.
 */

DaQOCloud.factory('breakerWarningService',function ($http) {
    return {
        createEchartsOption:function(result){
            return toption = {
                title : {
                    text: '故障报警统计：',
                    subtext: ''
                },
                tooltip : {
                    trigger: 'axis'
                },
                legend: {
                    data:['发生报警次数']
                },
                toolbox: {
                    show : true,
                    feature : {
                        //mark : {show: true},
                        //dataView : {show: true, readOnly: false},
                        magicType : {show: true, type: ['line', 'bar']},
                        //restore : {show: true},
                        //saveAsImage : {show: true}
                    }
                },
                calculable : true,
                xAxis : [
                    {
                        type : 'category',
                        data : result.xAxis
                    }
                ],
                yAxis : [
                    {
                        type : 'value'
                    }
                ],
                series : [
                    {
                        name:'发生报警次数',
                        type:'bar',
                        data:result.data,
                        markPoint : {
                            data : [
                                {type : 'max', name: '最大值'},
                                {type : 'min', name: '最小值'}
                            ]
                        },
                        markLine : {
                            data : [
                                {type : 'average', name: '平均值'}
                            ]
                        }
                    }
                ]
            };
        },
        createEcharts : function (targetdiv,data) {
            $that = this;
            var ops = {'width':'auto','height':'auto'};
            var myChart = echarts.init(targetdiv,'macarons',ops);
            var option = $that.createEchartsOption(data);
            myChart.setOption(option);
            /*require(
                [
                    'echarts',
                    'echarts/theme/macarons',
                    'echarts/chart/bar',
                    'echarts/chart/line'
                ],
                function (ec,theme) {
                    myChart = ec.init(targetdiv,theme);
                    var option = $that.createEchartsOption(data);
                    myChart.setOption(option);
                }
            );*/
            return myChart;
        },
        objectListToValueList :function(list,key){
            var listvalue = [];
            list.forEach(function(e){
                listvalue.push(e[key]);
            })
            return listvalue;
        },
        getCountData : function(deviceid,startDate,endDate,kinds){
            $that = this;
            console.log(kinds.join(","));
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/ProjectManage/getBreakCount?deviceid="+deviceid+
                "&startDate="+startDate+ "&endDate="+endDate+"&kinds="+kinds.join(","),{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                var map = {data:$that.objectListToValueList(result,'breakCount'),xAxis:$that.objectListToValueList(result,'breakDateTime')}
                return map;
            })
        },
        getBreakInfoData : function(projectId,deviceid,startDate,endDate,kinds){
            console.log(kinds.join(","));
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/ProjectManage/getDeviceBreakInfo?projectid="+projectId+"&deviceid="+deviceid+
                "&startDate="+startDate+ "&endDate="+endDate+"&kinds="+kinds.join(","),{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            })
        },
        downloadCsv:function(a_target,data,head){
            a_target.attr("download","故障报警日志_"+new Date().getTime()+".csv");
            var text = "";
            var text = head.join(',') + '\n'
            data.forEach(function(e,i){
                var temp = [i,e.strBreakTimeStamp,e.strDeviceName,e.strBreakKind,e.strBreakContent]
                text = text + temp.join(',')+'\n'
            })
            text = encodeURIComponent(text)
            a_target.attr("href","data:text/csv;charset=utf-8,\ufeff" + text);
            a_target.click();

        }
    }
})
