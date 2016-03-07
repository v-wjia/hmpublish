<!DOCTYPE html>
<html lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>
<head>
<?php print $head; ?>
<title><?php print $head_title; ?></title>
<?php print $styles; ?>
<?php print $scripts; ?>
<!--  <script src="js/jquery-1.11.3.js"></script>-->
  <script>
    jQuery(function(){
      jQuery(".level-1").data("state","open");
      jQuery(".level-1").click(function(){
        $that = jQuery(this).siblings();
        $that.toggle();
        if(jQuery(this).data("state") == "open"){
          jQuery(this).data("state","close");
          jQuery(this).find(".array").css("-webkit-transform","rotate(-45deg)");
          jQuery(this).find(".array").css("-o-transform","rotate(-45deg)");
          jQuery(this).find(".array").css("-ms-transform","rotate(-45deg)");
          jQuery(this).find(".array").css("-moz-transform","rotate(-45deg)");
          jQuery(this).find(".array").css("transform","rotate(-45deg)");
        }else{
          jQuery(this).data("state","open");
          jQuery(this).find(".array").css("-webkit-transform","rotate(45deg)");
          jQuery(this).find(".array").css("-o-transform","rotate(45deg)");
          jQuery(this).find(".array").css("-ms-transform","rotate(45deg)");
          jQuery(this).find(".array").css("-moz-transform","rotate(45deg)");
          jQuery(this).find(".array").css("transform","rotate(45deg)");
        }
      })
    })
  </script>
<!--[if lt IE 9]><script src="<?php print base_path() . drupal_get_path('theme', 'cqtheme160301') . '/js/html5.js'; ?>"></script><![endif]-->
</head>
<body class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>