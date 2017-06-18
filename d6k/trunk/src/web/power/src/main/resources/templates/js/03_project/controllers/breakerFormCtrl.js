/**
 * Created by lojoso on 2017/1/11.
 */
DaQOCloud.controller('breakerFormCtrl',['$scope','$stateParams','$state','$rootScope','$filter',function ($scope, $stateParams, $state,$rootScope,$filter) {
    $scope.targetDevice = $filter('filter')($filter('filter')($scope.pjInfoList,{'intDBPkPjinfoid':$stateParams.projectId})[0].lstDeviceInfo,{'intDeviceid':$stateParams.breakerId})[0]
    $scope.selectedParams = [];
    $scope.chooseThis = function(obj,name){
        if($(obj.currentTarget).hasClass("chose_c")){
            $(obj.currentTarget).removeClass("chose_c");
        }else{
            $(obj.currentTarget).addClass("chose_c");
        }
    }
    $scope.refleshSelectedList = function($event,obj){
        if($filter('filter')($scope.selectedParams,{"strDBKey":obj}).length < 1){
            $scope.selectedParams.push($filter('filter')($scope.targetDevice.lstDBParams,{"strDBKey":obj})[0]);
        }
    }
    $scope.delSelectParam = function(){
        $("span[class*=chose_c]").each(function(index,element) {
            $scope.selectedParams.splice($.inArray($filter('filter')($scope.selectedParams,{"strDBKey":$(element).attr("id").slice(7,$(element).attr("id").length)})[0],
                $scope.selectedParams),1)
        });
    }
    $scope.createTable = function(){
        var startTime = new Date($scope.startDateValue).valueOf();
        var endTime = new Date($scope.endDateValue).valueOf() + 86400000;
        if(endTime - startTime > 172800000){
            $scope.timeSpace = 86400000
        }else {
            $scope.timeSpace = 3600000
        }
        fetch("http://172.16.42.37:8080/DaQOCloudSystem/ProjectManage/getDeviceReport?deviceDbkey="+$scope.targetDevice.strMongoDBCollectionName
            +"&timeList="+this.createTimeList(startTime,endTime,$scope.timeSpace)
            +"&columns="+this.getColumnsList()
            +"&space="+$scope.timeSpace,{method:'post'}).then(function(res) {
            console.log("Response succeeded?", JSON.stringify(res.ok));
            return res.json();
        }).then(function(result){
            $scope.reportDataList = result;
            $scope.$apply($scope.reportDataList);
            return result;
        });
    }

    $scope.createTimeList = function (start,end,space) {
        var timelist = [];
        while(start < end){
            timelist.push(start)
            start += space;
        }
        return timelist;
    }

    $scope.getColumnsList = function () {
        var colList = [];
        $scope.selectedParams.forEach(function(e){
            colList.push(e.strDBKey);
        })
        return colList;
    }

    $scope.getColumnsNameList = function () {
        var colList = [];
        $scope.selectedParams.forEach(function(e){
            colList.push(e.strTitle);
        })
        return colList;
    }

    $scope.downLoadCsv = function(){
        $that = this;
        $("#downloadBtn").attr("download","报表_"+new Date().getTime()+".csv");
        var head = this.getColumnsNameList();
        var headkey =this.getColumnsList();
        var text = "";
        var text = "时间间隔,"+head.join(',') + '\n'
        $scope.reportDataList.forEach(function(e,i){
            var temp = [];
            temp.push($that.myFormat($scope.startDateValue.valueOf()+(i*$scope.timeSpace)))
            headkey.forEach(function(el,ind){
                temp.push(e[el]);
            });
            text = text + temp.join(',')+'\n'
        })
        text = encodeURIComponent(text)
        $("#downloadBtn").attr("href","data:text/csv;charset=utf-8,\ufeff" + text);
        $("#downloadBtn").click();
    }

    $scope.myFormat = function (date) {
        var d = new Date(date)
        var year = d.getFullYear();
        var month = (d.getMonth()+1) < 10 ? "0"+(d.getMonth()+1) : (d.getMonth()+1);
        var day = d.getDate() < 10 ? "0" + d.getDate() : d.getDate();
        var hour = d.getHours() < 10 ? "0" + d.getHours() : d.getHours();
        var min = d.getMinutes() < 10 ? "0" + d.getMinutes() : d.getMinutes();
        var sec = d.getSeconds() < 10 ? "0" + d.getSeconds() : d.getSeconds();
        return year+"-"+month+"-"+day+" "+hour+":"+min+":"+sec;
    }
}])