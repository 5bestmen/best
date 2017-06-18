/**
 * Created by reed on 2016/12/23.
 */

DaQOCloud.controller('homeCtrl',['$scope','$stateParams','$state','homeServices',function ($scope, $stateParams, $state, homeServices) {
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

        $('body').removeClass();
        $('body').addClass('bg');

        $(document).click(function(e){
            if(e.target.tagName == 'A' && $(e.target).hasClass('hrefto')){
                $state.go('project').then(function(){
                    $state.go('project.tabs',{'projectId':3});
                });
                $scope.$apply();
            }
        })
    });
    function resizeHeight() {
        var mainHeight = document.documentElement.clientHeight - 150;
        $("#left").css("height", mainHeight);
        $("#mid").css("height", mainHeight);
        $("#right").css("height", mainHeight);
        $("#huabu").css("height", mainHeight);
        $("#huabu1").css("height", mainHeight-19);
        $("#huabu2").css("height", mainHeight - 19);
    }

    var  map,infoWindow,notcloseWindow;
    map = homeServices.addPlugin(homeServices.setFeatures(homeServices.createMap('mid',17),['bg', 'point']));

    //homeServices.makeMakers(map,infoWindow,notcloseWindow);
}]);