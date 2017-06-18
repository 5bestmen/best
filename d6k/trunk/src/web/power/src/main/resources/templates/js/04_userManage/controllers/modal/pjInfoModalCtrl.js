/**
 * Created by lojoso on 2017/1/22.
 */
DaQOCloud.controller('pjInfoModalCtrl',['$scope','$http','$rootScope','$filter','$location','$modalInstance','paramObj',function($scope, $http, $rootScope,$filter,$location,$modalInstance,paramObj) {

    $scope.project = paramObj;

    $scope.cancel = function(){
        $modalInstance.dismiss('cancel'); // 退出
    };

    $scope.ok = function(){
        $modalInstance.close($scope.project); //关闭并返回当前选项
    };

}]);