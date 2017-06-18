/**
 * Created by reed on 2017/1/6.
 */
DaQOCloud.controller('headerCtrl',['$scope','$stateParams','$state',function ($scope, $stateParams, $state) {
    //
    $scope.logout = function(){
        $('body').removeClass();
        $('body').addClass('body');
        $state.go('login');
    }


    $scope.gotoProjectPage = function () {
        $state.go('project')
            .then(function(){
                $state.go('project.tabs',{'projectId':3})
                    .then(function () {
                        $state.go('project.tabs.run');
                });
        });
    }

    $scope.gotoUserManagePage = function () {
        $state.go('userManage').then(function () {
            $state.go("userManage.padChange");
        })
    }

    $scope.goBack = function(){
        if(jQuery.isEmptyObject($scope.logininfo)){
            $('body').removeClass();
            $('body').addClass('body');
            $state.go("login");
        }else{
            $('body').removeClass();
            $('body').addClass('bg');
        }
    }
    // $scope.goBack();

    var meaObject = $.fn.GeneralMeasurement.init();
    // todo:后续取消注释
    //meaObject.load();
}]);