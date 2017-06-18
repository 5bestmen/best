/**
 * Created by reed on 2017/1/10.
 */

DaQOCloud.controller('userInfoMangeCtrl',['$scope','$stateParams','$state','userInfoMangeService','$filter','$modal',function ($scope, $stateParams, $state,userInfoMangeService,$filter,$modal) {

    function resizeHeight() {
        var mainHeight = document.documentElement.clientHeight - 150;
        $("#left").css("height", mainHeight);
        $("#mid").css("height", mainHeight);
        $("#right").css("height", mainHeight);
        $("#huabu").css("height", mainHeight);
        $("#huabu1").css("height", mainHeight-19);
        $("#huabu2").css("height", mainHeight - 19);
    }

    $(function() {
        $('.unselected').bind("click",function() {
            var src = $('.unselected').attr("src");
            if(src == "img/unselected.png"){
                $('.unselected').attr("src","img/selected.png");
            }else{
                $('.unselected').attr("src","img/unselected.png");
            }
        });
        resizeHeight();
        $(window).resize(function() {
            resizeHeight();
        });

        var scrollTop = 0;
        var scrollLeft = 0;
        $('#huabu1').scroll(function () {

            if(scrollLeft < $(this).scrollLeft() || scrollLeft > $(this).scrollLeft()) {
                scrollLeft = $(this).scrollLeft();
                var dir = scrollLeft < $(this).scrollLeft() ? 1 : -1;
                $('#horizontal-ruler').scrollLeft(19 * dir + scrollLeft)
            }

            if(scrollTop < $(this).scrollTop() || scrollTop > $(this).scrollTop()) {
                scrollTop = $(this).scrollTop();
                var dir = scrollTop < $(this).scrollTop() ? 1 : -1;
                $('#vertical-ruler').scrollTop(19 * dir + scrollTop)

            }
        });
    });

    userInfoMangeService.getUserList($scope.logininfo.userInfoID.intPkUserInfoId,$scope.logininfo.userInfoID.strPassword).then(function(res){
        $scope.UserinfoListSource = angular.copy(res);
        $scope.UserinfoList = res;
        $scope.$apply($scope.UserinfoList)
    })

    $scope.doFilter = function () {
        var kind = $("#filterKind").val();
        var content = $("#filterContent").val();
        $scope.UserinfoList = $filter('filter')($scope.UserinfoListSource,JSON.parse('{"'+kind+'":"'+content+'"}'),false)

    }

    $scope.doClear = function(){
        $scope.UserinfoList = $scope.UserinfoListSource;
    }

    $scope.deleteUser = function(id){
        userInfoMangeService.deleteUser($scope.logininfo.userInfoID.intPkUserInfoId,$scope.logininfo.userInfoID.strPassword,id).then(function () {
            userInfoMangeService.getUserList($scope.logininfo.userInfoID.intPkUserInfoId,$scope.logininfo.userInfoID.strPassword).then(function(res){
                $scope.UserinfoListSource = angular.copy(res);
                $scope.UserinfoList = res;
                $scope.$apply($scope.UserinfoList)
            })
        })
    }

    $scope.getPjList = function(l){
        var list = [];
        l.forEach(function (e,i) {
            list.push(e.strPjName);
        })
        return list.join(',');
    }

    $scope.getImeiList = function(l){
        var list = [];
        l.forEach(function (e,i) {
            list.push(e.strImei);
        })
        return list.join(',');
    }

    $scope.openUserInfoModal = function(){
        var modalInstance = $modal.open({
            templateUrl : 'resources/html/04_userManage/modal/userInfoModal.html',
            controller : 'userInfoModalCtrl',
            backdrop:false,
            resolve : {
                paramObj : function(){
                    return angular.copy({});
                }
            }
        });

        modalInstance.result.then(function (result) {
            userInfoMangeService.AddUser(result).then(function(){
                userInfoMangeService.getUserList($scope.logininfo.userInfoID.intPkUserInfoId,$scope.logininfo.userInfoID.strPassword).then(function(res){
                    $scope.UserinfoListSource = angular.copy(res);
                    $scope.UserinfoList = res;
                    $scope.$apply($scope.UserinfoList)
                })
            });
            console.log(result);
        },function (reason) {
            console.log(reason);
        });
    }

    $scope.editUserInfoModal = function(target){
        var modalInstance = $modal.open({
            templateUrl : 'resources/html/04_userManage/modal/userInfoModal.html',
            controller : 'userInfoModalCtrl',
            backdrop:false,
            resolve : {
                paramObj : function(){
                    return {adminid:$scope.logininfo.userInfoID.intPkUserInfoId,pwd:$scope.logininfo.userInfoID.strPassword,targetid:target};
                }
            }
        });

        modalInstance.result.then(function (result) {
            userInfoMangeService.updateUser(result).then(function(){
                userInfoMangeService.getUserList($scope.logininfo.userInfoID.intPkUserInfoId,$scope.logininfo.userInfoID.strPassword).then(function(res){
                    $scope.UserinfoListSource = angular.copy(res);
                    $scope.UserinfoList = res;
                    $scope.$apply($scope.UserinfoList)
                })
            });
            console.log(result);
        },function (reason) {
            console.log(reason);
        });
    }
}]);