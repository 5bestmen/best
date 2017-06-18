/**
 * Created by reed on 2017/1/11.
 */

DaQOCloud.controller('workProConfigureCtrl',['$scope','$stateParams','$state',function ($scope, $stateParams, $state) {

    $(function() {
        $('.unselected').bind("click",function() {
            var src = $('.unselected').attr("src");
            if(src == "resources/img/unselected.png"){
                $('.unselected').attr("src","resources/img/selected.png");
            }else{
                $('.unselected').attr("src","resources/img/unselected.png");
            }
        });
        resizeHeight();
        $(window).resize(function() {
            resizeHeight();
        });
        $('#huabu li').click(function() {
            if($(this).hasClass("nav_active")) {
                $(this).next('ul').slideUp();
                $(this).removeClass("nav_active");
                $(this).find('img').attr("src", "resources/templates/img/nav_tag02.png");
            } else {
                $(this).next('ul').slideDown();
                $(this).addClass("nav_active");
                $(this).find('img').attr("src", "resources/templates/img/nav_tag.png");
            }

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

    function resizeHeight() {
        var mainHeight = document.documentElement.clientHeight - 150;
        $("#left").css("height", mainHeight);
        $("#mid").css("height", mainHeight);
        $("#right").css("height", mainHeight);
        $("#huabu").css("height", mainHeight);
        $("#huabu1").css("height", mainHeight-19);
        $("#huabu2").css("height", mainHeight - 19);
    }

}]);
