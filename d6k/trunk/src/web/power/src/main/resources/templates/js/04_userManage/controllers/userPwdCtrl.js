/**
 * Created by lojoso on 2017/1/16.
 */

DaQOCloud.controller('userPwdCtrl',['$scope','$stateParams','$state','loginService',function ($scope, $stateParams, $state,loginService) {

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

    $scope.updatePwd = function(){
        fetch("http://172.16.42.37:8080/DaQOCloudSystem/Login101/updateUserPwd?userid=" + $scope.logininfo.userInfoID.intPkUserInfoId
            +"&pwd="+($scope.newPwd == $scope.reNewPwd ? $scope.newPwd : "")
            +"&oldPwd="+$scope.oldPwd,{method:'post'}).then(function(res) {
            console.log("Response succeeded?", JSON.stringify(res.ok));
            return res.json();
        }).then(function(result){
            return result;
        });
    }
}]);