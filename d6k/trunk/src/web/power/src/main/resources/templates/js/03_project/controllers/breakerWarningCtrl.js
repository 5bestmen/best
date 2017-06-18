/**
 * Created by lojoso on 2017/1/11.
 */

DaQOCloud.controller('breakerWarningCtrl',['$scope','$stateParams','$state','$rootScope','breakerWarningService',function ($scope, $stateParams, $state,$rootScope,breakerWarningService) {

    $scope.deviceBreakList = [];
    var list = [1,2,3];
    $scope.refreshList = function(val){
        if($.inArray(val, list) >= 0){
            list.splice($.inArray(val, list),1)
        }else{
            list.push(val);
        }
    }

    $scope.getDateList = function(startDate,endDate){
        var list = [];
        list.push(startDate.getFullYear()+"-"+((startDate.getMonth()+1) < 10 ? "0"+(startDate.getMonth()+1):(startDate.getMonth()+1))+"-"
            +(startDate.getDate() < 10 ? "0"+startDate.getDate() : startDate.getDate()));
        while (startDate.toLocaleString() != endDate.toLocaleString()){
            startDate = new Date(startDate.setDate(startDate.getDate()+1))
            list.push(startDate.getFullYear()+"-"+((startDate.getMonth()+1) < 10 ? "0"+(startDate.getMonth()+1):(startDate.getMonth()+1))+"-"
                +(startDate.getDate() < 10 ? "0"+startDate.getDate() : startDate.getDate()));
        }
        return list;
    }

    $scope.createReport = function () {
        var startDate = new Date($scope.startDateValue);
        var endDate = new Date($scope.endDateValue);
        console.log(list.sort())
        breakerWarningService.getCountData($stateParams.breakerId,startDate.getFullYear()+"-"+(startDate.getMonth()+1)
            +"-"+startDate.getDate(), endDate.getFullYear()+"-"+(endDate.getMonth()+1)+"-"+endDate.getDate(),list.sort()).then(function(result){
            var valuelist = [],datelist = $scope.getDateList(startDate,endDate);
            datelist.forEach(function(e,i){
                if($.inArray(e, result.xAxis) >= 0){
                    valuelist.push(result.data[$.inArray(e,result.xAxis)]);
                }else{
                    valuelist.push(0);
                }
            })
            result.data = valuelist;
            result.xAxis = datelist;

            breakerWarningService.createEcharts($(".chart02")[0],result);
        });
        breakerWarningService.getBreakInfoData($stateParams.projectId,$stateParams.breakerId,startDate.getFullYear()+"-"+(startDate.getMonth()+1)
            +"-"+startDate.getDate(), endDate.getFullYear()+"-"+(endDate.getMonth()+1)+"-"+endDate.getDate(),list.sort()).then(function(result){
            $scope.deviceBreakList = result;
            $scope.$apply($scope.deviceBreakList);
        })
    }

    $scope.downloadData = function(){
        breakerWarningService.downloadCsv($(".build02"),$scope.deviceBreakList,["No","时间","设备","故障类型","故障详细"]);
    }

}]);