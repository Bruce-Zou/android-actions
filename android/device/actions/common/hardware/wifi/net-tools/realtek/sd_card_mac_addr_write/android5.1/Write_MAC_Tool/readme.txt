�������ṩ�˶ԡ�WIFI mac��ַ���͡�����mac��ַ��������

==== ʹ�÷��� ====
�ѱ����ߵ�Ŀ¼copy��sd��������������Ļ����У���FileBroswer�����Ŀ¼�������write_mac.sh������


==== �����ļ� ====
1. �ļ���ʽ
ʾ�����£�
mac_type=wifi_mac_addr
wifi_addr_min=10:01:FF:00:00:00
wifi_addr_cur=10:01:FF:B1:00:2A
wifi_addr_max=10:01:FF:FF:FF:FF
mac_type=bt_mac_addr
bt_addr_min=00:01:BB:00:00:00
bt_addr_cur=00:01:BB:77:8C:20
bt_addr_max=00:01:BB:FF:FF:FF

2. ��ʽ˵����
(01) mac_type ������дmac��ַ�����ͣ������ǡ�wifi_mac_addr����btaddr���������߼�֧��ͬʱ��дwifi mac��ַ������mac��ַ��Ҳ֧�ֵ�����д����mac��ַ��wifi mac��ַ��
     ��wifi_mac_addr�� -- wifi��mac��ַ��
     ��btaddr��  -- ������Mac��ַ��
     ʾ��1) mac_type=wifi_mac_addr
            ��Ҫ��дwifi mac ��ַ
     ʾ��2)  mac_type=bt_mac_addr
            ��Ҫ��д���� mac ��ַ

(02) wifi MAC��ַ(wifi_mac_addr)
wifi_addr_min=10:01:FF:00:00:00  -- ��ʾMAC����ʼ��ַ(16����)����10:01:FF:00:00:00��
wifi_addr_cur=10:01:FF:B1:00:2A  -- ��ʾ��ǰ�ᱻд������ĵ�ַ����10:01:FF:B1:00:2A��
wifi_addr_max=10:01:FF:FF:FF:FF  -- ��ʾMAC����ֹ��ַ(16����)����10:01:FF:FF:FF:FF��
     a) wifi_mac_addr��btaddr�ĸ�ʽҪ�����ơ�
     b) min,cur��max�ĸ�ʽ���붼����xx:xx:xx:xx:xx:xx����ʽ�����У�x��16���Ʒ��ţ���x����0123456789ABCDEFabcdef �����xx��ǰ��һ��Ϊ0��֧��ֱ��ʡ��ǰ��һ��0��
     c) cur��ֵ�������[min,max]֮�䡣
     d��wifi MAC��ַǰ24λΪ����id��ͬһ����ʹ�õĳ���idӦ����ͬ������Ҫ��wifi_addr_min��wifi_addr_cur��wifi_addr_maxǰ��һ�루MAC��ַ10:01:FF:00:00:00ǰ��һ�������Ϊ10:01:FF:��������Ӧ����ͬ��
     e) Ҫ���һ���ֽڵĺ�����λ����Ϊ0��Ҳ����aa:bb:cc:dd:ee:ff ��aa���������λΪ0���ܱ�4��������
     f) ��ַ������Ϊȫ0��
     g) ִ��һ��д�����֮��cur��ֵ��+1��


(03) ������ַ(btaddr)
bt_addr_min=00:01:BB:00:00:00  -- ��ʾ��������ʼ��ַ(16����)����00:01:BB:00:00:00��
bt_addr_cur=00:01:BB:77:8C:20  -- ��ʾ��ǰ�ᱻд������ĵ�ַ����00:01:BB:77:8C:20��
bt_addr_max=00:01:BB:FF:FF:FF  -- ��ʾ�����Ľ�����ַ(16����)����00:01:BB:FF:FF:FF��
     a) min,cur��max�ĸ�ʽ���붼����xx:xx:xx:xx:xx:xx����ʽ�����У�x��16���Ʒ��ţ���x����0123456789ABCDEFabcdef��
     b) cur��ֵ�������[min,max]֮�䡣
     c) BT Mac address��һ��byte������0,��������ֽڲ�����Ϊ 0x9e8b00 ~ 0x9e8b3f
     d) ��ַ������Ϊȫ0
     e) ִ��һ��д�����֮��cur��ֵ��+1��



***�����cur���Ѿ����ڡ�end�������о�����ʾ�Ѿ��ﵽ���ֵ��
***�����cur���Ѿ����ڡ�end�������б�����ʾ����û����ʾ�������ԭ�򣬿ɲ鿴��Ŀ¼��log�ļ����µ�log�ļ���
***��ʾ��Success����ʱ���ʾִ���Ѿ�����������ֱ�Ӱο������ط�Ҫ�ȴ���ʾ��ʧ��




