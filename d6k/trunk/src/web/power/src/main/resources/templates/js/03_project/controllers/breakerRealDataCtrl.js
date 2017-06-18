/**
 * Created by lojoso on 2017/1/9.
 */

DaQOCloud.controller('breakerRealDataCtrl',['$scope','$stateParams','$state','$rootScope',function ($scope, $stateParams, $state,$rootScope) {


    $scope.initCardAndData = function () {
        return fetch("http://172.16.42.37:8080/DaQOCloudSystem/ProjectManage/getDeviceParamCards?deviceid="+$stateParams.breakerId,{method:'post'}).then(function(res) {
            console.log("Response succeeded?", JSON.stringify(res.ok));
            return res.json();
        }).then(function(result){
            return result;
        });
    }
    $scope.initCardAndData().then(function(result){
        $scope.deviceCardList = result.lstDBParams;
        $scope.deviceName = result.strDeviceName;
        $scope.$apply($scope.deviceCardList);
        initcard();
    });
    $scope.timeIntrval = setInterval(function () {
        $scope.initCardAndData().then(function(result){
            result.lstDBParams.forEach(function(e){
                $("#devicecard_"+e.strDBKey).html(e.strDBValue);
            })
        });
    },3000)

    $rootScope.$watch('$state.current.name',function (obj1,obj2) {
        if(obj1 !== obj2){
            clearInterval($scope.timeIntrval)
        }
    })
}]);
