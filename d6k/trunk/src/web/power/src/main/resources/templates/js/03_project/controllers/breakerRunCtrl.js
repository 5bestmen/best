/**
 * Created by lojoso on 2017/1/9.
 */

DaQOCloud.controller('breakerRunCtrl',['$scope','$stateParams','$state','$rootScope',function ($scope, $stateParams, $state,$rootScope) {
    console.info($stateParams.breakerId);
    $(function() {
        $scope.initPageView();
        $scope.deviceBreakList = [];
        $rootScope.socket = new SockJS('http://172.16.42.37:8080/DaQOCloudSystem/websocketserver');
        $rootScope.stompClient = Stomp.over($rootScope.socket);
        $rootScope.stompClient.connect({}, function(frame) {
            console.log('Connected: ' + frame);
            $rootScope.stompClient.subscribe('/user/'+$scope.logininfo.userInfoID.intPkUserInfoId+'_'+$stateParams.projectId+'_'+$stateParams.breakerId+'/message', function(greeting){
                console.log(greeting.body);
                if($scope.deviceBreakList.length == 50){
                    $scope.deviceBreakList.pop();
                }
                $scope.deviceBreakList.unshift(JSON.parse(greeting.body));
                $scope.$apply($scope.deviceBreakList);
            });
        });
    });

    fetch("http://172.16.42.37:8080/DaQOCloudSystem/ProjectManage/getDeviceBreaks?deviceid="+$stateParams.breakerId,{method:'post'}).then(function(res) {
        console.log("Response succeeded?", JSON.stringify(res.ok));
        return res.json();
    }).then(function(result){
        $scope.deviceBreakList = result;
        $scope.$apply($scope.deviceBreakList);
        return result;
    });
}]);

/*        fetch("http://172.16.42.37:8080/DaQOCloudSystem/SmartCloud102/getDeviceBreakInfo?projectid="+$stateParams.projectId+"&deviceid="+$stateParams.breakerId+
 "&startDate="+startDate.getFullYear()+"-"+(startDate.getMonth()+1)+"-"+startDate.getDate()+
 "&endDate="+endDate.getFullYear()+"-"+(endDate.getMonth()+1)+"-"+endDate.getDate(),{method:'post'}).then(function(res) {
 console.log("Response succeeded?", JSON.stringify(res.ok));
 return res.json();
 }).then(function(result){
 $scope.deviceBreakList = result;
 $scope.$apply($scope.deviceBreakList);
 return result;
 });*/