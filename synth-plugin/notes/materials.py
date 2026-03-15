from math import sqrt

# material properties source:   engineeringtoolbox.com

# bar - aluminum
bar_youngs_modulus = 70e9
bar_poissons_ratio = 0.334

# soft rubber ------------------------------------------------------------------
mal_youngs_modulus = 0.01e9 
mal_poissons_ratio = 0.49        
mal_diameter = 0.019       
mal_radius = mal_diameter / 2.0

contact_modulus = 1.0 / ((1 - mal_poissons_ratio ** 2) / mal_youngs_modulus + 
                         (1 - bar_poissons_ratio ** 2) / bar_youngs_modulus)

#hertz_spring_constant = (4 / 3) * contact_modulus * sqrt(mal_radius)
#print(f'{"Soft rubber:":<16}K = {hertz_spring_constant:e}')  
print(f'{"Soft rubber:":<16}E* = {contact_modulus:e}')  


# mallet - rubber (hard) -------------------------------------------------------
mal_youngs_modulus = 0.1e9      # Pa
mal_poissons_ratio = 0.49        
mal_diameter = 0.019            # m
mal_radius = mal_diameter / 2.0

contact_modulus = 1.0 / ((1 - mal_poissons_ratio ** 2) / mal_youngs_modulus + 
                         (1 - bar_poissons_ratio ** 2) / bar_youngs_modulus)

#hertz_spring_constant = (4 / 3) * contact_modulus * sqrt(mal_radius)
#print(f'{"Hard rubber:":<16}K = {hertz_spring_constant:e}')     # 1.707337e07

print(f'{"Hard rubber:":<16}E* = {contact_modulus:e}')

# mallet - ABS -----------------------------------------------------------------
mal_youngs_modulus = 2.25e9     # Pa
mal_poissons_ratio = 0.364       
mal_diameter = 0.019            # m
mal_radius = mal_diameter / 2.0

contact_modulus = 1.0 / ((1 - mal_poissons_ratio ** 2) / mal_youngs_modulus + 
                         (1 - bar_poissons_ratio ** 2) / bar_youngs_modulus)

#hertz_spring_constant = (4 / 3) * contact_modulus * sqrt(mal_radius)
#print(f'{"ABS:":<16}K = {hertz_spring_constant:e}') 
print(f'{"ABS:":<16}E* = {contact_modulus:e}') 

# mallet - ABS (hardest) -------------------------------------------------------
#mal_youngs_modulus = 3.1e9      # Pa
#mal_poissons_ratio = 0.364       
#mal_diameter = 0.019            # m
#mal_radius = mal_diameter / 2.0
#
#contact_modulus = 1.0 / ((1 - mal_poissons_ratio ** 2) / mal_youngs_modulus + 
#                         (1 - bar_poissons_ratio ** 2) / bar_youngs_modulus)
#
#hertz_spring_constant = (4 / 3) * contact_modulus * sqrt(mal_radius)
#
#print(f'{"ABS:":<16}K = {hertz_spring_constant:e}') 
#
